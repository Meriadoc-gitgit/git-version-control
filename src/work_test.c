#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src.h"
#include "bash.h"

int main(void) {
  WorkFile* wf1 = createWorkFile("WF1");
  wf1->hash = "hihi";
  wf1->mode = 100;
  
  printf("%s\n",wfts(wf1));
  
  return 0;
}