@ECHO OFF

SET A=0
:loop
TIMEOUT 2 > NUL 2>&1
FOR /F "tokens=*" %%a in ('adb shell getprop sys.boot_completed') do SET A=%%a
IF NOT "%A%"=="1" goto loop

TIMEOUT 2 > NUL 2>&1