@echo off
rem Build the Game Gear ROM: banks\legend_of_zelda_links_awakening.gg
rem
rem   ..\src\*.c          unbanked code (always mapped)
rem   ..\src\bankN\*.c    code and constants placed in ROM bank N (N >= 2)
rem   ..\assets\          data packed into banks by assets2banks (from bank %FIRST_ASSET_BANK%)
rem
rem New .c files are picked up automatically.

setlocal enabledelayedexpansion
cd /d "%~dp0"

rem warning 336 is from SMSlib.h (SMS_SRAM[] declared without a size), not project code
set CFLAGS=-c -mz80 --opt-code-speed -DTARGET_GG --peep-file peep-rules.txt -I. -I..\libs --disable-warning 336
set FIRST_ASSET_BANK=2
set RELS=
set BANKOPTS=

del /q *.rel 2>nul

rem --- assets ------------------------------------------------------------
rem (skipped while ..\assets holds nothing but assets2banks.cfg)
dir /b /a-d ..\assets | findstr /v /i /x "assets2banks.cfg" >nul
if not errorlevel 1 (
    assets2banks ..\assets --compile --firstbank=%FIRST_ASSET_BANK%
    if errorlevel 1 goto :error
)
rem one header for all assets (bank numbers change as assets are added)
(for %%h in (bank*.h) do type %%h) > assets.h

rem --- unbanked code -----------------------------------------------------
for %%f in (..\*.c) do (
    echo %%~nxf
    sdcc %CFLAGS% %%f -o %%~nf.rel
    if errorlevel 1 goto :error
    set RELS=!RELS! %%~nf.rel
)

rem --- banked code: src\bankN ------------------------------------------------
for /d %%d in (..\bank*) do (
    set B=%%~nd
    set N=!B:bank=!
    for %%f in (%%d\*.c) do (
        echo %%~nd\%%~nxf
        sdcc %CFLAGS% --codeseg BANK!N! --constseg BANK!N! %%f -o %%~nd_%%~nf.rel
        if errorlevel 1 goto :error
        set RELS=!RELS! %%~nd_%%~nf.rel
    )
)

rem --- assets2banks output (bankN.rel) -------------------------------------
for %%f in (bank*.rel) do (
    echo %%~nf | findstr /r "^bank[0-9][0-9]* $" >nul && set RELS=!RELS! %%f
)

rem --- bank placement: bank N lives in slot 2 ($8000) ------------------------
rem (only for banks that have something in them)
for /l %%n in (2,1,63) do (
    set USED=
    if exist bank%%n.rel set USED=1
    if exist ..\bank%%n set USED=1
    if defined USED (
        set /a ADDR=%%n*65536+32768
        call :hex !ADDR! HEXADDR
        set BANKOPTS=!BANKOPTS! -Wl-b_BANK%%n=0x!HEXADDR!
    )
)

echo === Linking ===
sdcc -o legend_of_zelda_links_awakening.ihx -mz80 --no-std-crt0 --data-loc 0xC000 %BANKOPTS% ^
    ..\libs\crt0_sms.rel %RELS% ..\libs\SMSlib_GG.lib ..\libs\PSGlib.lib
if errorlevel 2 goto :error

echo === Converting to a Game Gear ROM ===
makesms -pp legend_of_zelda_links_awakening.ihx ..\legend_of_zelda_links_awakening.gg
if errorlevel 1 goto :error

echo === Cleaning up ===
del /q *.lst *.sym *.lk *.map *.noi *.ihx *.rel *.asm 2>nul

echo Build completed: legend_of_zelda_links_awakening.gg
exit /b 0

:error
echo Build failed!
exit /b 1

:hex
rem :hex value outvar - convert a decimal number to hex
set /a "v=%1"
set "h="
set "digits=0123456789ABCDEF"
:hexloop
set /a "d=v %% 16"
set /a "v=v / 16"
set "h=!digits:~%d%,1!!h!"
if !v! GTR 0 goto :hexloop
set "%2=!h!"
exit /b 0