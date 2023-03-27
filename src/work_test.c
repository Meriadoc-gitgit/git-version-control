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
  WorkFile* work=stwf("hehe.c\tizeulgflr\t500");
  WorkTree* wt1 = initWorkTree();
  WorkTree* wt2 = stwt("src.c\tlahfb\t708\nhihi.c\taoiefh\t600\nhi.m\taoiefh\t600\nhii.k\taoiefh\t600");
  WorkTree* wt3 = initWorkTree();

  /* Actions */
  printf("\t\tAJOUT\n");
  appendWorkTree(wt1,"src.c","oadsuifghipug",705);
  appendWorkTree(wt1,"src.txt","oadsuifghipug",715);
  appendWorkTree(wt1,"src1.c","oadsuifghipug",710);
  printf("n of wt1: %d\n",wt1->n);
  
  appendWorkTree(wt3,"hiha.c","oadsuifghipug",705);
  appendWorkTree(wt3,"hihi.txt","oadsuifghipug",715);
  appendWorkTree(wt3,"hihi.c","oadsuifghipug",710);
  printf("n of wt3: %d\n",wt3->n);
  printf("\t\t FIN AJOUT\n");

  /* Affichage */
  printf("AFFICHAGE\n");
  printf("wf1: %s\n",wfts(wf1));
  printf("wf2: %s\n",wfts(wf2));
  printf("workfile: %s\n",wfts(work));
  printf("wt1:\n%s\n",wtts(wt1));
  printf("wt2:\n%s\n",wtts(wt2));
  printf("wt3:\n%s\n",wtts(wt3));

  //printf("wt2.1: %s\n",wfts(&(wt2->tab[0])));
  //printf("n of wt2: %d\n",wt2->n);

  wttf(wt1,"src.txt");
  WorkTree* wt4 = ftwt("src.txt");
  printf("wt4:\n%s\n",wtts(wt4));

  printf("FIN AFFICHAGE\n");

  printf("chmod de [src.c]: %d\n",getChmod("src.c")); 

  WorkFile* wf5 = createWorkFile("work_test.o");

  printf("test position inWT on wt4 with src1.c: %d\n",inWorkTree(wt4,"src1.c"));

  wttf(wt4,"file_test.txt");

  WorkTree* wt6 = ftwt("file_test.txt");
  printf("\ntest de ftwt on wt6: \n%s\n",wtts(wt6));

  /*LIBERATION*/

  libererWorkFile(wf5);
  //libererWorkFile(wf2);
  //libererWorkFile(work);
  //libererWorkTree(&wt1);
  //libererWorkTree(&wt2);
  //libererWorkTree(&wt3);
  //libererWorkTree(wt4);

  return 0;
}