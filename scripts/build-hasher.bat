SET ROOT_DIR=%1
SET OUTPUT_DIR=%~2

@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
@REM call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
@REM call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

cd /D %ROOT_DIR%

cl.exe %ROOT_DIR%\hasher.c ^
/Fe%OUTPUT_DIR%\hasher.exe ^
/DUNICODE /D_UNICODE /DWIN32 /EHsc /nologo