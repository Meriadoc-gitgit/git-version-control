#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src.h"
#include "bash.h"

int main(void) {

  /* Initialisation */
  List* L = initList();
  //List* L1 = ftol("src.txt");
  List* L2 = stol("chain1|chain2");

  //printf("L1: %s\n",ltos(L1));

  /* List copied from the main one */
  List* LT0 = L; List* LT2 = L2;

  Cell *C = buildCell("main.c");
  //Cell *C1 = listGet(LT1,1);
  Cell *C2 = searchList(L2,"chain1");

  /* Insertion */
  insertFirst(L,C);
  //insertFirst(L,C1);

  /* printf*/
  printf("Affichage de List:\n");
  printf("L: %s\n",ltos(L));
  //printf("L1: %s\n",ltos(L1));
  printf("L2: %s\n\n",ltos(L2));

  printf("src: %s\n",ltos(listdir("/Users/vuhoangthuyduong/Documents/GitHub/GIT-version-control/src")));

  printf("\nAffichage de Cell:\n");
  printf("C: %s\n",ctos(C));
  //printf("C1: %s\n",ctos(C1));
  printf("C2: %s\n",ctos(C2));

  Cell* C3 = listGet(L,1);
  printf("test de listGet sur C3: %s\n",ctos(C3));

  Cell* C4 = searchList(L2,"chain1");
  printf("test de searchList sur C4: %s\n",ctos(C4));
  /* FILE */
  ltof(L2,"file_test.txt");

  List* L3 = ftol("file_test.txt");
  printf("test de ftol sur L3: %s\n",ltos(L3));

  /* Free */
  free(C->data);
  free(L); free(C);

  /* SRC */
  //printf("hash: %s\n",sha256file("src.txt"));
  return 0;
}