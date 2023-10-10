#include "Core.h"
#include "Win32.h"

SEC(text, B) VOID Entry(VOID) {
  INSTANCE instance = {0};

  instance.Modules.kernel32 = LdrModulePeb(HASH_KERNEL32);
  instance.Modules.ntdll = LdrModulePeb(HASH_NTDLL);

  if (instance.Modules.kernel32 != NULL) {
    // Load needed functions
    instance.Win32.LoadLibraryA =
        LdrFunction(instance.Modules.kernel32, 0xb7072fdb);

    // Load needed Libraries
    instance.Modules.msvcrt =
        instance.Win32.LoadLibraryA(GET_SYMBOL("msvcrt"));

    if (instance.Modules.msvcrt != NULL) {
      instance.Win32.system =
          LdrFunction(instance.Modules.msvcrt, 0xcff4a7ca);
    }
  }

  // ------ Code ------
  instance.Win32.system(GET_SYMBOL("start notepad.exe"));
}