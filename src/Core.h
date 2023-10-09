
#include <windows.h>

#include "Macros.h"

UINT_PTR GetRIP(VOID);

typedef struct {
  // Win32 APIs
  struct {  // change to whatever you need to call
    WIN32_FUNC(system); 
    WIN32_FUNC(LoadLibraryA);
  } Win32;

  struct {
    // Basics
    HMODULE kernel32;
    HMODULE ntdll;

    // Libraries need to load when call the functions above(`struct Win32`)
    HMODULE msvcrt;
  } Modules;

} INSTANCE, *PINSTANCE;