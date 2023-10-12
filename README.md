# ShellcodeTemplate

### Notice
- Windows 64bit only;
- core source code from this brilliant repo [Cracked5pider/ShellcodeTemplate](https://github.com/Cracked5pider/ShellcodeTemplate);

### Status
- [x] add Visual Studio Code tasks to build & extract shellcode;
- [x] add example to use the shellcode;
- [ ] using MSVC toolset instead of GNU;

### Requirements
- GNU toolset, please see [MinGW](https://www.mingw-w64.org/downloads/);
- Python, require the latest version;
  - please also install pip module of `pefile`: `pip install pefile`;
- Visual Studio, C/C++ dev environment required;
- NASM, chose the latest version to [download](https://www.nasm.us/pub/nasm/releasebuilds/);

### Usage
- First, find out what library and what function you need to use;
  - For example, you want to call `system()` in the target process: 
    1. add lib `msvcrt` module in `struct INSTANCE`;
    2. add function `system` in `struct Win32`, both structs are defined in file `core.h`;
    3. load the `msvcrt` with `instance.Win32.LoadLibraryA(GET_SYMBOL("msvcrt"))` function;
    4. get the address of the target function `system` with `LdrFunction(instance.Modules.msvcrt, 0xcff4a7ca)` function;
    5. finnally, call the `system("start notepad.exe")` with `instance.Win32.system(GET_SYMBOL("start notepad.exe"))` function, please see file `entry.c` for more details;
- Calculate the function hash by using
  ```cmd
  hasher.exe "system" "LoadLibraryA"
  // result:
  // [+] Hashed SYSTEM ==> 0xcff4a7ca
  // [+] Hashed LOADLIBRARYA ==> 0xb7072fdb
  ```
  - use the `build hasher.exe` task to build the `hasher.exe` application;
- Get the shellcode by using `build PE` task;
  - the shellcode will be saved in `out\shellcode.bin` file;
- Test the shellcode
  - check the `examples` folder for shellcode injection examples;
