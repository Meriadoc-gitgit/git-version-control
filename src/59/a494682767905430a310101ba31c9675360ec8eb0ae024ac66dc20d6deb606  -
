#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "src.h"
#include "bash.h"

int main(int argc, char* argv[]) {

  switch (case_break(argv[1])) {
    case 1:
      initRefs();
      initBranch();
      break;

    case 2:
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

    case 3:
      createUpdateRef(argv[2],argv[3]);
      break;

    case 4:
      deleteRef(argv[2]);
      break;

    case 5: 
      for (int i=2;i<argc;i++) 
        myGitAdd(argv[2]);
      break;

    case 6:
      system("rm .add");
      break;

    case 7:
      printf("Zone de preparation : \n");
      if (file_exists(".add")) {
        WorkTree* wt = ftwt(".add");
        printf("%s\n",wtts(wt));
      }
      break;

    case 8:
      if (strcmp(argv[3],"-m")==0)
        myGitCommit(argv[2],argv[4]);
      else 
        myGitCommit(argv[2],NULL);
      break;

    case 9: 
      printf("%s\n",getCurrentBranch());
      break;

    case 10: 
      if (!branchExists(argv[2]))
        createBranch(argv[2]);
      else printf("The branch already exists.\n");
      break;

    case 11: 
      if (!branchExists(argv[2]))
        printf("The branch does not exist.\n");
      else printBranch(argv[2]);
      break;

      case 12: 
        if (!branchExists(argv[2]))
        printf("The branch does not exist.\n");
      else myGitCheckoutBranch(argv[2]);
      break;

    case 13: 
      myGitCheckoutCommit(argv[2]);
      break;

  }
  return 0;
}