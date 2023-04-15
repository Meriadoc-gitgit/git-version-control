
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "src.h"
#include "bash.h"

int main(void) {

  //Commit* c1 = createCommit(sha256file("src.c"));

  initRefs();
  
  initBranch();
  printf("check if branch master exists: %d\n",branchExists("master"));
  createBranch("Feature1");

  //printf("blobCommit de c1: %s\n", blobCommit(c1));
  //printf("hashToPath de c1: %s\n", hashToFile(blobCommit(c1)));

  //blobCommit(createCommit(sha256file("bash.c")));
  //createUpdateRef("Feature0",blobCommit(c1));

  printBranch("Feature0");

  List* L = branchList("Feature0");
  printf("%s\n",ltos(L));

  List* L1 = getAllCommits();
  printf("%s\n",ltos(L1));

  myGitAdd("src.c");
  myGitCommit("Feature0","test");
}