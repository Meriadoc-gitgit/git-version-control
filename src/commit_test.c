#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "src.h"
#include "bash.h"

int main(void) {
  
  /* Instantiation */
  kvp* k1 = createKeyVal("Duong","hehehe");
  
  /* Affichage */
  printf("k1: %s\n", kvts(k1));
  printf("stkv test on k1: %s\n",kvts(stkv(kvts(k1))));

  /* Desallocation */
  freeKeyVal(k1);


  /* COMMIT test */
  Commit* c = initCommit();
  commitSet(c,"Duong","hehehe");
  commitSet(c,"Duong","hehehe");

  for (int i=hash("Duong");i<MAX_INPUT;i++) {
    if (c->T[i]) 
      printf("test commit %d: %s\n",i,kvts(c->T[i]));
  }

  return 0;
}