#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include "src.h"
#include "bash.h"

int main(void) {
  
  /* AFFICHAGE */
  printf("Files dans l'ensemble du projet:\n%s\n",ltos(listdir("/Users/vuhoangthuyduong/Documents/GitHub/GIT-version-control")));

  printf("Le repertoire src existe dans ce projet: %d\n",file_exists("src.c"));
  printf("main.c in the project: %s\n",ctos(searchList(listdir("/Users/vuhoangthuyduong/Documents/GitHub/GIT-version-control"),"main.c")));

  char cwd[PATH_MAX];
  getcwd(cwd,PATH_MAX); printf("cwd: %s\n",cwd);
  //cp("src.tmp","src.c");

  printf("hashToPath de src.c: %s\n",hashToPath("oweifbpowbgpoabguwabg"));

  //blobFile("file_test.txt");

  return 0;
}