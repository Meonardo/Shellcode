
#include <windows.h>

#include "Macros.h"

UINT_PTR GetRIP(VOID);

typedef struct {
  struct {
    WIN32_FUNC(system);
    WIN32_FUNC(LoadLibraryA);
  } Win32;

  struct {
    // Basics
    HMODULE Kernel32;
    HMODULE Ntdll;

    HMODULE MSVCRT;
  } Modules;

} INSTANCE, *PINSTANCE;