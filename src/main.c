#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src.h"
#include "bash.h"

int main(void) {

  /* Initialisation */
  List* L = initList();
  List* L1 = ftol("src.txt");
  List* L2 = stol("chain1|chain2|");
  printf("L2: %s\n\n",ltos(L2));

  Cell *C = buildCell("main.c");
  //Cell *C1 = listGet(L1,1);
  //Cell *C2 = searchList(L1,"test");

  /* Insertion */
  insertFirst(L,C);
  //insertFirst(L,C1);

  /* printf*/
  printf("Affichage de List:\n");
  printf("L: %s\n",ltos(L));
  printf("L1: %s\n",ltos(L1));
  //printf("L2: %s\n",ltos(L2));

  /*
  printf("L2(1): %s\n",ctos(*L2));
  printf("L2(2): %s\n",ctos((*L2)->next));
  printf("L2(3): %s\n",ctos(((*L2)->next)->next));
  */

  printf("\nAffichage de Cell:\n");
  printf("C: %s\n",ctos(C));
  //printf("C1: %s\n",ctos(C1));
  //printf("C2: %s\n",ctos(C2));

  /* FILE */
  ltof(L2,"ltof.txt");

  /* Free */
  free(C->data);
  free(L); free(C);

  // src
  //printf("hash: %s\n",sha256file("src.txt-XXXXXX"));
  return 0;
}