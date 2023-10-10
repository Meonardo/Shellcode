#include <windows.h>

#include "macros.h"

PVOID LdrModulePeb(UINT_PTR module_hash);
PVOID LdrFunction(UINT_PTR module, UINT_PTR proc_hash);