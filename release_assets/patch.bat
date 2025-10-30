@ECHO OFF

set /p Version=<version.txt

IF EXIST "root" (
ECHO Please remove "root" folder.
PAUSE
EXIT
)

IF EXIST "%1" ( GOTO :HASFILE ) ELSE ( GOTO :NOFILE )

:HASFILE
GOTO :MAIN

:NOFILE
ECHO Pikmin 2 iso file not found. Please drag a Pikmin 2 iso onto the bat file.
PAUSE
EXIT


:MAIN
ECHO P2GZ v.%Version% Installer!
ECHO Extracting Pikmin 2 iso...
"%~dp0nodtool.win64" extract -c 2 %1 "%~dp0root"
ECHO iso extracted...

xcopy /y "%~dp0Patch\root\files" "%~dp0root\files" /E
xcopy /y "%~dp0Patch\root\sys" "%~dp0root\sys" /E

ECHO Now building new iso...
"%~dp0nodtool.win64" makegcn -c 2 "%~dp0root" "p2gz-%Version%%~x1"
rd /s /q "%~dp0root"
ECHO done! :)
PAUSE
