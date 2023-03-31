#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "src.h"
#include "bash.h"

int main(int argc, char* argv[]) {

  switch (hash(argv[1])) {
    case 2090370361:
      initRefs();
      break;

    case -1542797282:
      printf("REFS : \n");
      if (file_exists(".refs")) {
        List* L = listdir(".refs");
        for (Cell* ptr=*L;ptr!=NULL;ptr=ptr->next) {
          if (strcmp(&ptr->data[0],".")==0)
            continue;
          char* content = getRef(ptr->data);
          printf(" %s \t %s \n",ptr->data,content);
        }
      }
      break;

    case 466421411:
      createUpdateRef(argv[2],argv[3]);
      break;

    case -549740574:
      deleteRef(argv[2]);
      break;

    case 193486030: 
      for (int i=2;i<argc;i++) 
        myGitAdd(argv[1]);
      break;

    case -215373182:
      system("rm .add");
      break;

    case -15473897:
      printf("Zone de preparation : \n");
      if (file_exists(".add")) {
        WorkTree* wt = ftwt(".add");
        printf("%s\n",wtts(wt));
      }
      break;

    case -152830482:
      if (strcmp(argv[3],"-m")==0)
        myGitCommit(argv[2],argv[4]);
      else 
        myGitCommit(argv[2],NULL);
      break;
  }
  return 0;
}