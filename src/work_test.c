#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src.h"
#include "bash.h"

int main(void) {

  /* Instanciation */
  WorkFile* wf1 = createWorkFile("WF1");
  wf1->hash = "hihi";
  wf1->mode = 100;
  WorkFile* wf2 = stwf("WF2\tsrc.c\t705");

  WorkTree* wt1 = initWorkTree();

  /* Actions */
  appendWorkTree(wt1,"src.c","oadsuifghipug",705);
  appendWorkTree(wt1,"src.txt","oadsuifghipug",705);
  appendWorkTree(wt1,"src.c","oadsuifghipug",705);

  /* Affichage */
  printf("wf1: %s\n",wfts(wf1));
  printf("wf2: %s\n",wfts(wf2));

  printf("wt1:\n%s\n",wtts(wt1));
  return 0;
}