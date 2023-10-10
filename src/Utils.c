#include "macros.h"
#include "utils.h"

SEC(text, B) UINT_PTR HashString(LPVOID string, UINT_PTR length) {
  ULONG Hash = 5381;
  PUCHAR Ptr = string;

  do {
    UCHAR character = *Ptr;

    if (!length) {
      if (!*Ptr) break;
    } else {
      if ((ULONG)(Ptr - (PUCHAR)string) >= length) break;
      if (!*Ptr) ++Ptr;
    }

    if (character >= 'a') character -= 0x20;

    Hash = ((Hash << 5) + Hash) + character;
    ++Ptr;
  } while (TRUE);

  return Hash;
}