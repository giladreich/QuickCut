@echo off

set VERSION=2.0.0
set NAME=QuickCutSetup
set ARCH=x86
set OUTPUT=%NAME%_%ARCH%_%VERSION%.exe
set PACKAGE_PATH=%~dp0packages\com.greich.quickcut.msvc.%ARCH%\
set DATA_PATH=%PACKAGE_PATH%data\
set META_PATH=%PACKAGE_PATH%meta\
set RCH_BIN=%~dp0..\bin\rch.exe

del /q %OUTPUT%
rd /s /q %DATA_PATH%
xcopy /s /y ..\..\..\QuickCut\Resources\ui\styles\theme_ubuntu.qss config\
xcopy /s /y ..\..\..\QuickCut\Resources\platform\windows\Manifest.txt resources\
xcopy /s /y ..\..\..\QuickCut\Resources\platform\windows\QuickCut.ico resources\
xcopy /s /y ..\..\..\..\docs\LICENSE %META_PATH%
xcopy /s /y ..\..\..\..\build\bin\QuickCut* %DATA_PATH%
xcopy /s /y ..\..\..\..\contrib\service\* %DATA_PATH%scripts\
windeployqt %DATA_PATH%
binarycreator -v -f -r resources/installer.qrc -c config/config.xml -p packages %OUTPUT%

cd resources
%RCH_BIN% -open %NAME%.rc -save %NAME%.res -action compile
cd /D %~dp0
%RCH_BIN% -open %OUTPUT% -save %OUTPUT% -res resources\%NAME%.res -action addoverwrite
del /q resources\%NAME%.res
