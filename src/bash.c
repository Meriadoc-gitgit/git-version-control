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
  char buff[100];
  sprintf(buff,"cat %s | sha256sum > %s",src,dst);
  return system(buff);
}

char* sha256file(char* file) {
  static char template[] ="/tmp/fileXXXXXX";
  char fname[1000];
  strcpy(fname,template);
  mkstemp(fname);

  hashFile(file,fname);
  char *hash = (char*)malloc(256*sizeof(char));
  FILE *f = fopen(fname,"r");
  fgets(hash,256,f);
  fclose(f);

  char cmd[10000]; 
  sprintf(cmd,"rm %s",fname);
  system(cmd);
  return hash;
}


List* listdir(char* root_dir) {
  List* L = initList();
  DIR* dp = opendir(root_dir);
  struct dirent* ep;
  if (dp) {
    while((ep = readdir(dp))) {
      insertFirst(L,buildCell(ep->d_name));
      List ptr = *L;
    }
    (void)closedir(dp);
  } else {
    perror("listdir: Could not opend the directory\n");
    return NULL;
  }
  return L;
}
int file_exists(char* file) {
  struct stat buffer;
  return (stat(file,&buffer)==0);
}
void cp(char* to, char* from) {
  if (!file_exists(from)) {
    printf("cp: Fichier demande n'existe pas\n");
    return;
  }
  char cmd[20000];
  sprintf(cmd,"cat %s > %s",from,to);
  system(cmd); return;
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
  char command[256];
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

struct stat st = {0};
char* blobWorkTree(WorkTree* wt) {
  static char template[] ="/tmp/XXXXXX";
  char *fname = strdup(template);
  mkstemp(fname); wttf(wt,fname);

  char *path = hashToPath(sha256file(fname));
  char *dir = (char*)malloc(2*sizeof(char));
  strncpy(dir,path,2);

  if (stat(sha256file(fname),&st)==-1) {
    char command[200];
    sprintf(command,"mkdir %s",dir);
    system(command);
  }
  sprintf(path,"%s.t",path);
  cp(path,fname); free(dir);
  return sha256file(fname);
}
char* concat(char* s1,char* s2) {
  char* dir = (char*)malloc(MAX_INPUT);
  strcat(dir,s1); strcat(dir,"/"); strcat(dir,s2);
  return dir;
}
char* saveWorkTree(WorkTree* wt,char* path) {//ok
  WorkFile WF; 
  for (int i=0;i<wt->n;i++) {
    WF = wt->tab[i]; 
    char* a_path = concat(path,WF.name);
    if (listdir(a_path)) {
      List* L = listdir(WF.name);
      WorkTree* newWT = initWorkTree(); 
      while(*L) {
        appendWorkTree(newWT,(*L)->data,sha256file((*L)->data),getChmod((*L)->data));
        *L = (*L)->next;
      }
      WF.hash = saveWorkTree(newWT,a_path);
      WF.mode = getChmod(a_path);
    } else {
      blobFile(a_path);
      WF.hash = sha256file(a_path);
      WF.mode = getChmod(a_path);
    }
  }
  return blobWorkTree(wt);
}
void restoreWorkTree(WorkTree* wt,char* path) {//ok
  WorkFile WF;
  for (int i=0;i<wt->n;i++) {
    WF = wt->tab[i];
    char* a_path = concat(path,WF.name);
    char* cpPath = hashToPath(WF.hash);
    if (strcmp(strstr(WF.hash,".t"),".t")==0) {
      strcat(a_path,".t");
      WorkTree* newWT = ftwt(cpPath);
      restoreWorkTree(newWT,a_path);
      setMode(getChmod(cpPath),a_path);
    } else {
      cp(a_path,cpPath); 
      setMode(getChmod(cpPath),a_path);
    }
  }
  return;
}
//verified!!


/* Part 3 - GESTION DES COMMITS */

/* Fonction de base */
char* blobCommit(Commit* c) {
  char* save = cts(c);
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

/* MANIPULATION DES REFERENCES */
void initRefs() {
  if (!file_exists(".refs")) {
    system("mkdir.refs");
  }
  system("echo > master"); 
  system("echo > HEAD");
  return;
}
void createUpdateRef(char* ref_name,char* hash) {
  FILE* f = fopen(ref_name,"w");
  if (!file_exists(".refs")) {
    initRefs();
    
  }
  if (!f) {
    char command[200]; sprintf(command,"echo > %s",ref_name);
    system(command);
  }
  fprintf(f,"%s",hash); fclose(f);
  return;
}
void deleteRef(char* ref_name) {
  char command[200]; sprintf(command,"rm %s",ref_name);
  system(command); return;
}
char* getRef(char* ref_name) {
  FILE* f = fopen(ref_name,"r");
  char buffer[MAX_INPUT];
  fgets(buffer,MAX_INPUT,f); 
  char* res = (char*)malloc(strlen(buffer));
  strcpy(res,buffer); fclose(f);
  return res;
}

/* SIMULATION */
void myGitAdd(char* file_or_folder) {
  if (!file_exists(".add")) 
    system("echo > .add");
  FILE* f = fopen(".add","w");
  WorkTree* WT = ftwt(".add");
  WorkFile* WF = stwf(file_or_folder);
  appendWorkTree(WT,WF->name,WF->hash,WF->mode);
  fprintf(f,"%s",wtts(WT));
  fclose(f);
  return;
}
void myGitCommit(char* branch_name,char* message) {
  if (!file_exists(".refs")) {
    printf("gitCommit: Initialiser d'abord les references du projet\n");
    return;
  }
  if (!file_exists(branch_name)) {
    printf("gitCommit: La branche n'existe pas\n");
    return;
  }
  char head[MAX_INPUT], branch[MAX_INPUT];
  FILE* f = fopen("HEAD","r"); fgets(head,MAX_INPUT,f); 
  FILE* g = fopen(branch_name,"r"); fgets(branch,MAX_INPUT,g);
  if (strcmp(head,branch)!=0) {
    printf("getCommit: HEAD doit pointer sur le dernier commit de la branche\n");
    return;
  }
  char cwd[PATH_MAX]; getcwd(cwd,PATH_MAX); sprintf(cwd,"%s/.refs",cwd);
  WorkTree* wt = ftwt(".add"); deleteRef(".add"); 
  char* h = saveWorkTree(wt,cwd);
  Commit* c = createCommit(h);
  if (strcmp(branch,"")!=0) 
    commitSet(c,"predecessor",branch);
  if (message!=NULL) 
    commitSet(c,"message",message);
  char* hc = blobCommit(c);
  fprintf(g,"%s",hc); fprintf(f,"%s",hc);
  return;
}