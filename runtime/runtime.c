#include <stdio.h>
#include <stdint.h>

int read_int() {
  int64_t val = 0;
  scanf("%ld", &val);
  return val;
}

void print(int64_t value) {
  printf("%ld\n", value);
}
