@echo off
assets2banks ..\assets --compile

sdcc -c -mz80 --peep-file peep-rules.txt ..\main.c

@if %errorlevel% NEQ 0 goto :EOF

sdcc -o legend_of_zelda_links_awakening.ihx -mz80 --data-loc 0xC000 -Wl-b_BANK2=0x8000 -Wl-b_BANK3=0x8000 -Wl-b_BANK4=0x8000 -Wl-b_BANK5=0x8000 -Wl-b_BANK6=0x8000 -Wl-b_BANK7=0x8000 -Wl-b_BANK8=0x8000 -Wl-b_BANK9=0x8000 -Wl-b_BANK10=0x8000 -Wl-b_BANK11=0x8000 -Wl-b_BANK12=0x8000 -Wl-b_BANK13=0x8000 -Wl-b_BANK14=0x8000 -Wl-b_BANK15=0x8000 -Wl-b_BANK16=0x8000 -Wl-b_BANK17=0x8000 -Wl-b_BANK18=0x8000 -Wl-b_BANK19=0x8000 -Wl-b_BANK20=0x8000 -Wl-b_BANK21=0x8000 -Wl-b_BANK22=0x8000 -Wl-b_BANK23=0x8000 -Wl-b_BANK24=0x8000 -Wl-b_BANK25=0x8000 -Wl-b_BANK26=0x8000 -Wl-b_BANK27=0x8000 -Wl-b_BANK28=0x8000 -Wl-b_BANK29=0x8000 -Wl-b_BANK30=0x8000 -Wl-b_BANK31=0x8000 -Wl-b_BANK32=0x8000 -Wl-b_BANK33=0x8000 -Wl-b_BANK34=0x8000 -Wl-b_BANK35=0x8000 -Wl-b_BANK36=0x8000 -Wl-b_BANK37=0x8000 -Wl-b_BANK38=0x8000 -Wl-b_BANK39=0x8000 -Wl-b_BANK40=0x8000 -Wl-b_BANK41=0x8000 --no-std-crt0 ..\crt0_sms.rel main.rel bank2.rel bank3.rel bank4.rel bank5.rel bank6.rel bank7.rel bank8.rel bank9.rel bank10.rel bank11.rel bank12.rel bank13.rel bank14.rel bank15.rel bank16.rel bank17.rel bank18.rel bank19.rel bank20.rel bank21.rel bank22.rel bank23.rel bank24.rel bank25.rel bank26.rel bank27.rel bank28.rel bank29.rel bank30.rel bank31.rel bank32.rel bank33.rel bank34.rel bank35.rel bank36.rel bank37.rel bank38.rel bank39.rel bank40.rel bank41.rel SMSlib_GG.lib ..\lib\PSGLib.rel
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