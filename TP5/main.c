#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "src.h"

int main(void) {
  Cell** L = initList();
  Cell* C = buildCell("test");

  insertFirst(L,C);
  insertFirst(L,buildCell("test2"));

  Cell* C2 = searchList(L,"test");
  Cell* C3 = listGet(L,2);
  char* test = ctos(C2);
  char* test2 = ctos(C3);
  printf("%s\n",test);
  printf("%s\n",test2);


  char* tmp = ltos(L);
  printf("%s\n",tmp);
  
  return 0;
}