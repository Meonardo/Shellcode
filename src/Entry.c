#include "Core.h"
#include "Win32.h"

SEC(text, B) VOID Entry(VOID) {
  INSTANCE Instance = {0};

  Instance.Modules.kernel32 = LdrModulePeb(HASH_KERNEL32);
  Instance.Modules.ntdll = LdrModulePeb(HASH_NTDLL);

  if (Instance.Modules.kernel32 != NULL) {
    // Load needed functions
    Instance.Win32.LoadLibraryA =
        LdrFunction(Instance.Modules.kernel32, 0xb7072fdb);

    // Load needed Libraries
    Instance.Modules.msvcrt =
        Instance.Win32.LoadLibraryA(GET_SYMBOL("msvcrt"));

    if (Instance.Modules.msvcrt != NULL) {
      Instance.Win32.system =
          LdrFunction(Instance.Modules.msvcrt, 0xcff4a7ca);
    }
  }

  // ------ Code ------
  Instance.Win32.system(GET_SYMBOL("start notepad.exe"));
}