@echo off
assets2banks ..\assets --compile

sdcc -c -mz80 --peep-file peep-rules.txt ..\main.c

@if %errorlevel% NEQ 0 goto :EOF

sdcc -o legend_of_zelda_links_awakening.ihx -mz80 --data-loc 0xC000 -Wl-b_BANK2=0x8000 -Wl-b_BANK3=0x8000 -Wl-b_BANK4=0x8000 -Wl-b_BANK5=0x8000 -Wl-b_BANK6=0x8000 -Wl-b_BANK7=0x8000 -Wl-b_BANK8=0x8000 -Wl-b_BANK9=0x8000 --no-std-crt0 ..\crt0_sms.rel main.rel bank2.rel bank3.rel bank4.rel bank5.rel bank6.rel bank7.rel bank8.rel bank9.rel SMSlib_GG.lib ..\lib\PSGLib.rel
ihx2sms legend_of_zelda_links_awakening.ihx ..\legend_of_zelda_links_awakening.gg

echo clean up files
del *.lst
del *.sym
del *.lk
del *.rel
del *.asm
del *.noi
del *.map
del *.ihx

java -jar ..\Emulicious\Emulicious.jar ..\legend_of_zelda_links_awakening.gg