#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>

#include "src.h"

/* Part 1 */
int hashFile(char* src, char *dst) {
  char res[200];
  sprintf(res,"sha256sum %s > %s",src,dst);
  return system(res);
}

char* sha256file(char* file) {
  static char template[] ="/tmp/XXXXXX";
  char *fname = strdup(template);
  mkstemp(fname);
  hashFile(file,fname);
  FILE *f = fopen(fname,"r");
  char *buffer = (char*)malloc(256*sizeof(char));
  fgets(buffer,256,f);
  fclose(f);
  return buffer;
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
  if (!getcwd(cwd,PATH_MAX)) {
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
  char res[200];
  sprintf(res,"cat %s > %s",from,to);
  system(res);
  return;
}
char* hashToPath(char* hash) {
  char res[(int)strlen(hash)+1];
  for (int i=0;i<2;i++) 
    res[i] = hash[i];
  res[2] = '/';
  for (int i=2;i<(int)strlen(hash);i++)
    res[i+1] = hash[i];

  char* str = (char*)malloc(sizeof(char));
  strcpy(str,res);
  return str;
}
void blobFile(char* file) {
  if (!file_exists(file)) {
    printf("blobFile: Fichier demande n'existe pas\n");
    return;
  }
  char *path = hashToPath(sha256file(file));
  char *dir = (char*)malloc(2*sizeof(char));
  strncpy(dir,path,2);
  char command[200];
  sprintf(command,"mkdir %s",dir);
  system(command);
  cp(path,file);
  return;
}



/* Part 2 */
int getChmod(const char* path) {
  struct stat ret;
  if (stat(path,&ret)==-1) return -1;
  return (ret.st_mode & S_IRUSR)|(ret.st_mode & S_IWUSR)|(ret.st_mode & S_IXUSR)| /*owner*/
    (ret.st_mode & S_IRGRP)|(ret.st_mode & S_IWGRP)|(ret.st_mode & S_IXGRP)| /*group*/
    (ret.st_mode & S_IROTH)|(ret.st_mode & S_IWOTH)|(ret.st_mode & S_IXOTH); /*other*/
}
void setMode(int mode,char* path) {
  char buff[100];
  sprintf(buff,"chmod %d %s",mode,path);
  system(buff);
}

char* blobWorkTree(WorkTree* wt) {
  char* save = wtts(wt);
  static char template[] ="/tmp/XXXXXX";
  char *fname = strdup(template);
  mkstemp(fname);

  FILE* f = fopen(fname,"w");
  fprintf(f,"%s",save); fclose(f);

  char *path = hashToPath(sha256file(fname));
  char *dir = (char*)malloc(2*sizeof(char));
  strncpy(dir,path,2);
  char command[200];
  sprintf(command,"mkdir %s",dir);
  system(command);
  sprintf(path,"%s.t",path);
  cp(path,fname);
  return sha256file(fname);
}
char* concat(char* s1,char* s2) {
  char* dir = (char*)malloc(MAX_INPUT);
  strcat(dir,s1); strcat(dir,"/"); strcat(dir,s2);
  return dir;
}
char* saveWorkTree(WorkTree* wt,char* path) {
  WorkFile WF; 
  for (int i=0;i<wt->n;i++) {
    WF = wt->tab[i]; 
    char* a_path = concat(path,WF.name);
    if (listdir(WF.name)) {
      List* L = listdir(WF.name);
      WorkTree newWT; 
      while(*L) {
        appendWorkTree(&newWT,(*L)->data,sha256file((*L)->data),getChmod((*L)->data));
        *L = (*L)->next;
      }
      saveWorkTree(&newWT,a_path);
      WF.hash = blobWorkTree(&newWT);
      WF.mode = getChmod(a_path);
    } else {
      blobFile(WF.name);
      WF.hash = sha256file(WF.name);
      WF.mode = getChmod(a_path);
    }
  }
  return blobWorkTree(wt);
}
void restoreWorkTree(WorkTree* wt,char* path) {
  WorkFile WF; char dir[26];
  for (int i=0;i<wt->n;i++) {
    WF = wt->tab[i];
    if (strcmp(strstr(WF.hash,".t"),".t")==0) {
      char* a_path = concat(path,WF.name);
      List* L = listdir(WF.name);
      WorkTree newWT; 
      while(*L) {
        appendWorkTree(&newWT,(*L)->data,sha256file((*L)->data),getChmod((*L)->data));
        *L = (*L)->next;
      }
      restoreWorkTree(&newWT,a_path);
    } else {
      char* a_path = concat(path,WF.name);
      cp(a_path,hashToPath(WF.hash)); setMode(WF.mode,a_path);
    }
  }
  return;
}



/* Part 3 - GESTION DES COMMITS */

/* Fonction de base */
char* blobCommit(Commit* c) {
  char* save = ftc(c);
  static char template[] ="/tmp/XXXXXX";
  char *fname = strdup(template);
  mkstemp(fname);

  FILE* f = fopen(fname,"w");
  fprintf(f,"%s",save); fclose(f);

  char *path = hashToPath(sha256file(fname));
  char *dir = (char*)malloc(2*sizeof(char));
  strncpy(dir,path,2);
  char command[200];
  sprintf(command,"mkdir %s",dir);
  system(command);
  sprintf(path,"%s.c",path);
  cp(path,fname);
  return sha256file(fname);
}