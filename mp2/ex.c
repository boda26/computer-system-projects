#include <stdio.h>
#include <stdint.h>
#include "lib/crc32.h"

int main() {
  char *buffer = "uiuc";

  uint32_t crc;

  crc = 0;
  crc32(buffer, 4, &crc);
  printf("%u\n", crc);

  crc = 0;
  crc32(buffer, 4, &crc);
  printf("%u\n", crc);

  crc = 0;
  crc32(buffer, 4, &crc);
  printf("%u\n", crc);

  return 0;
}

