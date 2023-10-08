SET ROOT_DIR=%1
SET OUTPUT_DIR=%~2

@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
@REM call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
@REM call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

cd /D %ROOT_DIR%

cl.exe /Zi /std:c++17 /DUNICODE /D_UNICODE /DWIN32 /EHsc /nologo /Fd%OUTPUT_DIR%\vc140.pdb /Fo%OUTPUT_DIR%\main.obj /Fe%OUTPUT_DIR%\Hasher.exe %ROOT_DIR%\Hasher.c