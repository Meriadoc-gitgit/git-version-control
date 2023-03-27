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

  printf("Le fichier src.c existe dans ce projet: %d\n",file_exists("src.c"));
  printf("main.c in the project: %s\n",ctos(searchList(listdir("/Users/vuhoangthuyduong/Documents/GitHub/GIT-version-control"),"main.c")));

  char cwd[PATH_MAX];
  getcwd(cwd,PATH_MAX); printf("cwd: %s\n",cwd);
  //cp("src.tmp","src.c");

  printf("hashToPath de src.c: %s\n",hashToPath("oweifbpowbgpoabguwabg"));

  //blobFile("file_test.txt");



  char* s1 = "hello";
  char* s2 = "world";
  char* s3 = concat(s1,s2);
  printf("s3: %s\n",s3);
  s3 = concat(s3,s1);
  printf("s3.2: %s\n",s3);


  printf("chmod du repertoire src dans le projet: %d\n",getChmod("/Users/vuhoangthuyduong/Documents/GitHub/GIT-version-control/src"));

  printf("chmod de ce projet: %d\n",getChmod("/Users/vuhoangthuyduong/Documents/GitHub/GIT-version-control"));

  printf("chmod du fichier src.c dans src dans le projet: %d\n",getChmod("/Users/vuhoangthuyduong/Documents/GitHub/GIT-version-control/src/src.c"));

  return 0;
}