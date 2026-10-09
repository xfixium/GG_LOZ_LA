@echo off
rem Build banks\legend_of_zelda_links_awakening.gg, then run it in Emulicious
call "%~dp0_compile.bat"
if errorlevel 1 exit /b 1
java -jar "%~dp0..\Emulicious\Emulicious.jar" "%~dp0..\legend_of_zelda_links_awakening.gg"