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
  WorkTree* wt2 = stwt("src.c\tlahfb\t708\nhihi.c\taoiefh\t600\nhi.m\taoiefh\t600\nhii.k\taoiefh\t600");

  /* Actions */
  appendWorkTree(wt1,"src.c","oadsuifghipug",705);
  appendWorkTree(wt1,"src.txt","oadsuifghipug",705);
  appendWorkTree(wt1,"src.c","oadsuifghipug",705);

  /* Affichage */
  printf("wf1: %s\n",wfts(wf1));
  printf("wf2: %s\n",wfts(wf2));

  //printf("wt1:\n%s\n",wtts(wt1));
  printf("wt2:\n%s\n",wtts(wt2));

  printf("wt2.1: %s\n",wfts(&(wt2->tab[0])));
  printf("n of wt2: %d\n",wt2->n);


  /* FILE */
  wttf(wt1,"src.txt");
  printf("%s\n",wtts(ftwt("src.txt")));
  
  return 0;
}