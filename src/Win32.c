#include "Win32.h"

#include <winternl.h>

#include "Macros.h"
#include "Utils.h"

// NOTE: The Module hash needs to be a hash from a wide string instead of an
// ansi string.
SEC(text, B) PVOID LdrModulePeb(UINT_PTR module_hash) {
  PLDR_DATA_TABLE_ENTRY module = (PLDR_DATA_TABLE_ENTRY)((PPEB)PPEB_PTR)
                                     ->Ldr->InMemoryOrderModuleList.Flink;
  PLDR_DATA_TABLE_ENTRY first_module = module;

  do {
    DWORD ModuleHash =
        HashString(module->FullDllName.Buffer, module->FullDllName.Length);

    if (ModuleHash == module_hash) return module->Reserved2[0];

    module = (PLDR_DATA_TABLE_ENTRY)module->Reserved1[0];
  } while (module && module != first_module);

  return INVALID_HANDLE_VALUE;
}

SEC(text, B) PVOID LdrFunction(UINT_PTR module, UINT_PTR func_hash) {
  PIMAGE_NT_HEADERS nt_header = NULL;
  PIMAGE_EXPORT_DIRECTORY exp_directory = NULL;
  PDWORD addr_of_funcs = NULL;
  PDWORD addr_of_names = NULL;
  PWORD addr_of_ordinals = NULL;
  PVOID func_addr = NULL;
  PCHAR func_name = NULL;
  ANSI_STRING ansi_string = {0};

  nt_header = C_PTR(module + ((PIMAGE_DOS_HEADER)module)->e_lfanew);
  exp_directory = C_PTR(module + nt_header->OptionalHeader
                                    .DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]
                                    .VirtualAddress);

  addr_of_names = C_PTR(module + exp_directory->AddressOfNames);
  addr_of_funcs = C_PTR(module + exp_directory->AddressOfFunctions);
  addr_of_ordinals = C_PTR(module + exp_directory->AddressOfNameOrdinals);

  for (DWORD i = 0; i < exp_directory->NumberOfNames; i++) {
    func_name = (PCHAR)module + addr_of_names[i];
    if (HashString(func_name, 0) == func_hash) {
      return C_PTR(module + addr_of_funcs[addr_of_ordinals[i]]);
    }
  }
}