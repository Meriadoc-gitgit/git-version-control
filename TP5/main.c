#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "src.h"

int main(void) {
  List* L = initList();
  Cell* C = buildCell("test");
  insertFirst(L,C);
  insertFirst(L,C);
  char* tmp = ltos(L);
  printf("%s\n",tmp);
  return 0;
}