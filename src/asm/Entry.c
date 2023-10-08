#include "Core.h"
#include "Win32.h"

SEC(text, B) VOID Entry(VOID) {
  INSTANCE Instance = {0};

  Instance.Modules.Kernel32 = LdrModulePeb(HASH_KERNEL32);
  Instance.Modules.Ntdll = LdrModulePeb(HASH_NTDLL);

  if (Instance.Modules.Kernel32 != NULL) {
    // Load needed functions
    Instance.Win32.LoadLibraryA =
        LdrFunction(Instance.Modules.Kernel32, 0xb7072fdb);

    // Load needed Libraries
    Instance.Modules.MSVCRT = Instance.Win32.LoadLibraryA(GET_SYMBOL("msvcrt"));

    if (Instance.Modules.MSVCRT != NULL) {
      Instance.Win32.system = LdrFunction(Instance.Modules.MSVCRT, 0xcff4a7ca);
    }
  }

  // ------ Code ------
  Instance.Win32.system(GET_SYMBOL(
      "start "
      "xxx"));
}