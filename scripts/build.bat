SET ROOT_DIR=%1
SET OUTPUT_DIR=%~2

@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
@REM call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
@REM call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

cd /D %ROOT_DIR%

SET TRARGET_NAME = Shellcode

echo "[*] Compile x64 PE file..."

@REM complie asm file first
nasm -f win64 %ROOT_DIR%\src\asm\x64\asm.s -o %OUTPUT_DIR%\asm.x64.o

clang.exe ^
    %ROOT_DIR%\src\Utils.c ^
    %ROOT_DIR%\src\Win32.c ^
    %ROOT_DIR%\src\Entry.c ^
    %OUTPUT_DIR%\asm.x64.o ^
    -o %OUTPUT_DIR%\%TRARGET_NAME%.exe ^
    -Os -fno-asynchronous-unwind-tables -nostdlib ^
    -fno-ident -fpack-struct=8 -falign-functions=1 ^
    -s -ffunction-sections -falign-jumps=1 -w ^
    -falign-labels=1 -Wl,-T%ROOT_DIR%\scripts\Linker.ld ^
    -Wl,-s,--no-seh,--enable-stdcall-fixup ^
    -I%src% ^
    -masm=intel