@echo off


::Ticketcsanner.exe should be put into the package folder (which is required by Qt Installer Framework)
::Provide temporary system variable "Path" for windeployqt.exe

set PATH_TO_QTDIR=%1
set PATH_TO_BINARYCREATOR=%2
set PATH_TO_PACKAGEFOLDER=%3

::Set Path="C:\Qt\Qt5.4.2\5.4\msvc2013\bin"
Set Path=%PATH_TO_QTDIR%\bin
Set TICKETSCANNER=%PATH_TO_PACKAGEFOLDER%\packages\TicketScanner\data\TicketScanner.exe

echo Collect all dependencies
%PATH_TO_QTDIR%\bin\windeployqt.exe --no-quick-import --no-translations --no-system-d3d-compiler %TICKETSCANNER%
echo ...Done


:: remove unnecessary plugins
rmdir /s /q %PATH_TO_PACKAGEFOLDER%\packages\TicketScanner\data\sensors\
rmdir /s /q %PATH_TO_PACKAGEFOLDER%\packages\TicketScanner\data\sqldrivers\
rmdir /s /q %PATH_TO_PACKAGEFOLDER%\packages\TicketScanner\data\sensorgestures\
rmdir /s /q %PATH_TO_PACKAGEFOLDER%\packages\TicketScanner\data\printsupport\




echo ...Creating package
%PATH_TO_BINARYCREATOR% --offline-only -c %PATH_TO_PACKAGEFOLDER%\config\config.xml -p %PATH_TO_PACKAGEFOLDER%\packages TScannerSetup.exe
echo ...Installator created





::Example of using
:: D:\Projects\TicketScanner>CreateInstallationPackage.bat  C:\Qt\Qt5.4.2\5.4\msvc2013   D:\Release\InstallerFramework\bin\binarycreator.exe   D:\Release\TSInstallerv1.0.2