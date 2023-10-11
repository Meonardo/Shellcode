#include "macros.h"
#include "utils.h"

SEC(text, B) UINT_PTR HashString(LPVOID string, UINT_PTR length) {
  ULONG Hash = 5381;
  PUCHAR ptr = string;

  do {
    UCHAR character = *ptr;

    if (!length) {
      if (!*ptr) break;
    } else {
      if ((ULONG)(ptr - (PUCHAR)string) >= length) break;
      if (!*ptr) ++ptr;
    }

    if (character >= 'a') character -= 0x20;

    Hash = ((Hash << 5) + Hash) + character;
    ++ptr;
  } while (TRUE);

  return Hash;
}