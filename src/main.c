#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include "src.h"
#include "bash.h"

int main(void) {
  printf("Files dans l'ensemble du projet:\n%s\n",ltos(listdir("/Users/vuhoangthuyduong/Documents/GitHub/GIT-version-control")));

  List* master = listdir("/Users/vuhoangthuyduong/Documents/GitHub/GIT-version-control/src");
  WorkTree* wt = initWorkTree();
  while(*master) {
    appendWorkTree(wt,(*master)->data,sha256file((*master)->data),getChmod((*master)->data));
    *master = (*master)->next;
  }

  blobFile("src.c");
  
  printf("blobwt test on wt: %s\n",blobWorkTree(wt));
  printf("savewt test on wt: %s\n",saveWorkTree(wt,"/users/Etu1/21110221/Documents/C/DATASTRUCT/GIT-version-control-main/src"));
  
  printf("%d\n",isFile("/users/Etu1/21110221/Documents/C/DATASTRUCT/GIT-version-control-main/src/src.c"));

  restoreWorkTree(wt,"src");
  
  cp("/Users/vuhoangthuyduong/Documents/GitHub/GIT-version-control/src/file_test.txt","/Users/vuhoangthuyduong/Documents/GitHub/GIT-version-control/src/src.txt");

  return 0;
}