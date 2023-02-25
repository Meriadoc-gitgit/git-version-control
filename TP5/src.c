#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>

#include "src.h"

int hashFile(char* src, char *dst) {
  char* res = "cat src.txt";
  //strcat(res,src);
  /*
  strcat(res,src);
  strcat(res," | sha256sum > ");
  strcat(res,dst);
  */
  return system(res);
}

char* sha256file(char* file) {
  char* src = strdup(file);
  int filedes = -1, count = 0;
  
  errno = 0;

  filedes = mkstemp(file);
  unlink(file);
  
  if (filedes<1) {
    printf("Echec de creation de file temporaire [%s]\n",strerror(errno));
    return NULL;
  } 
  else printf("File temporaire [%s] cree\n",file);

  errno = 0;

  hashFile(src, file);
  /* Read the data */
  char buffer[256];
  FILE* f = fopen(file,"r");
  if (!f) {
    printf("Erreur lors de l'ouverture\n");
    return NULL;
  }
  fgets(buffer, 256, f);
  fclose(f);
  
  return NULL;
}
List* listdir(char* root_dir) {
  List* L = initList();
  DIR* dp = opendir(root_dir);
  struct dirent* ep;
  if (dp) {
    while((ep = readdir(dp))) 
      insertFirst(L,buildCell(ep->d_name));
  }
  return L;
}
int file_exists(char* file) {
  char cwd[PATH_MAX];
  if (!getcwd(cwd,sizeof(cwd))) {
    perror("getcwd() error\n");
    return 1;
  }
  List* L = listdir(cwd);
  return searchList(L,file)!=NULL;
}
void cp(char* to, char* from) {
  if (!file_exists(from)) {
    printf("Fichier demande n'existe pas\n");
    return;
  }
  char* res = "cat ";
  strcat(res,from);
  strcat(res," > ");
  strcat(res,to);
  system(res);
  return;
}
char* hashToPath(char* hash) {
  char* res = "";
  for (int i=0;i<2;i++) 
    strcat(res,&hash[i]);
  strcat(res,"/");
  for (int i=2;i<(int)strlen(hash);i++)
    strcat(res,&hash[i]);
  return res;
}
/* Check later */
void blobFile(char* file) {
  system("mkdir INSTANT");
  char* rst = "cat ";
  strcat(rst,file);
  strcat(rst," > file.tmp");
  system(rst);
  return;
}