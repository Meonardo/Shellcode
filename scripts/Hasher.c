#include <ctype.h>
#include <stdio.h>

long Hash(char* string) {
  unsigned long Hash = 5381;
  int c;

  while ((c = *string++)) {
    Hash = ((Hash << 5) + Hash) + c;
  }

  return Hash;
}

void ToUpperString(char* temp) {
  // Convert to upper case
  char* s = temp;
  while (*s) {
    *s = toupper((unsigned char)*s);
    s++;
  }
}

int main(int argc, char** argv) {
  if (argc < 2) 
    return 0;

  for (int i = 1; i < argc; i++) {
    ToUpperString(argv[i]);
    printf("[+] Hashed %s ==> 0x%lx\n", argv[i], Hash(argv[i]));
  }

  return 0;
}
