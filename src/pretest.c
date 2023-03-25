#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "bash.h"
#include "src.h"

int main(void) {
  unsigned char i = 43;
  printf("%lu\n",hash(&i));

  return 0;
}