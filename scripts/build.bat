@echo off

SET ROOT_DIR=%1
SET OUTPUT_DIR=%~2

cd %ROOT_DIR%

echo "[*] Compile x64 PE file..."

@REM complie asm file first
nasm -f win64 %ROOT_DIR%\src\asm\x64\asm.s -o %OUTPUT_DIR%\asm.x64.o

@REM compile c files
gcc.exe ^
    %ROOT_DIR%\src\Utils.c ^
    %ROOT_DIR%\src\Win32.c ^
    %ROOT_DIR%\src\Entry.c ^
    %OUTPUT_DIR%\asm.x64.o ^
    -o %OUTPUT_DIR%\Shellcode.exe ^
    -Os -fno-asynchronous-unwind-tables -nostdlib ^
    -fno-ident -fpack-struct=8 -falign-functions=1 ^
    -s -ffunction-sections -falign-jumps=1 -w ^
    -falign-labels=1 -fPIC -Wl,-T%ROOT_DIR%\scripts\Linker.ld ^
    -Wl,-s,--no-seh,--enable-stdcall-fixup ^
    -I%src% ^
    -masm=intel