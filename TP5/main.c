#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cell.h"
#include "src.h"

int main(void) {
  List* L = initList();
  Cell* C = buildCell("test");

  insertFirst(L,C);
  insertFirst(L,buildCell("test2"));

  //char* tmp = ltos(L);
  //printf("tmp:%s\n",tmp);

  ltof(L,"dst.txt");
  List* L2 = stol("chain1|chain2|chain3");
  List* L3 = ftol("dst.txt");
  char* tmp = ltos(L3);
  printf("\ntmp:%s\n",tmp);

  //int hash = hashFile("src.txt","dst.txt");
  //printf("%d\n",hash);
  return 0;
}