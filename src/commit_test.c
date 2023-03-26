#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  return 0;
}