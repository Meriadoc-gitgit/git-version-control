#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "src.h"
#include "bash.h"

int main(int argc, char* argv[]) {

  if (!strcmp(argv[1],"init")) {
    initRefs();
    initBranch();
  }
  else if (!strcmp(argv[1],"list-refs")) {
    printf("REFS : \n");
    if (file_exists(".refs")) {
      List* L = listdir(".refs");
      for (Cell* ptr=*L;ptr!=NULL;ptr=ptr->next) {
        if (strcmp(&ptr->data[0],".")==0)
          continue;
        printf(" %s \t %s \n",ptr->data,getRef(ptr->data));
      }
    }
  }
  else if (!strcmp(argv[1],"create-ref")) 
    createUpdateRef(argv[2],argv[3]);

  else if (!strcmp(argv[1],"delete-ref"))
    deleteRef(argv[2]);
  else if (!strcmp(argv[1],"add")) {
    for (int i=2;i<argc;i++) 
      myGitAdd(argv[i]);
  }
  else if (!strcmp(argv[1],"clear-add"))
    system("rm .add");

  else if (!strcmp(argv[1],"list-add")) {
    printf("Preparation zone : \n");
    if (file_exists(".add")) {
      WorkTree* wt = ftwt(".add");
      printf("%s\n",wtts(wt));
    }
  }
  else if (!strcmp(argv[1],"commit")) {
    myGitCommit(argv[2],argv[4]); //pas necessaire de comparer "-m" avec le 3e argument de la commande, au risque des fautes de segmentation !!
  }
  else if (!strcmp(argv[1],"get-current-branch")) 
    printf("%s\n",getCurrentBranch());

  else if (!strcmp(argv[1],"branch")) {
    if (!branchExists(argv[2]))
      createBranch(argv[2]);
    else printf("The branch already exists.\n");
  }
  else if (!strcmp(argv[1],"branch-print")) {
    if (!branchExists(argv[2]))
      printf("The branch does not exist.\n");
    else printBranch(argv[2]);
  }
  else if (!strcmp(argv[1],"checkout-branch")) {
    if (!branchExists(argv[2]))
      printf("The branch does not exist.\n");
    else myGitCheckoutBranch(argv[2]);
  }
  else if (!strcmp(argv[1],"checkout-commit")) 
    myGitCheckoutCommit(argv[2]);
  
  else if (!strcmp(argv[1],"merge")) {
    List* conflicts = merge(argv[2],argv[3]);
    if (!conflicts) 
      printf("Branch %s successfully merged with the current branch %s\n",argv[2],getCurrentBranch());
    else {
      printf("WARNING: CONFLICTS DETECTED\nPlease choose among 3 options:\n1. Keep all files from the current branch\n2. Keep all files from %s\n3. Choose manually\n",argv[2]);
      
      int choice; 
      scanf("Enter chosen option: %d\n",&choice);
      switch (choice) {
      case 1: 
        createDeletionCommit(getCurrentBranch(),conflicts,argv[3]);
        conflicts = merge(argv[2],argv[3]);
        if (!conflicts) 
          printf("Branch %s successfully merged with the current branch %s\n",argv[2],getCurrentBranch());
        break;
        
      case 2: 
        createDeletionCommit(argv[2],conflicts,argv[3]);
        conflicts = merge(argv[2],argv[3]);
        if (!conflicts) 
          printf("Branch %s successfully merged with the current branch %s\n",argv[2],getCurrentBranch());
        break;

      case 3: 
        printf("CONFLICTS REVIEW\n");
        List* current = initList();
        List* remote = initList(); 
        int option; 
        while(*conflicts) {
          printf("%s - Please choose among 2 options below:\n1. Keep current branch version\n2. Keep remote branch version\n",(*conflicts)->data);
          scanf("Enter your choice: %d",&option);
          
          switch (option) {
            case 1: 
              insertFirst(current,buildCell((*conflicts)->data));
              break;
            case 2: 
              insertFirst(remote,buildCell((*conflicts)->data));
              break;
          }
          createDeletionCommit(getCurrentBranch(),current,argv[3]);
          createDeletionCommit(argv[2],remote,argv[3]);
        }
        break;
        
      default:
        printf("Warning: Invalid option.\nPlease re-enter chosen option\n");
        scanf("Enter chosen option: %d\n",&choice);
        break;
      }
    }
    conflicts = merge(argv[2],argv[3]);
    if (!conflicts) 
      printf("Successfully merged %s & %s\n",getCurrentBranch(),argv[2]);
    else printf("Warning: Failed to merge %s & %s\n",getCurrentBranch(),argv[2]);
  }
}