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
  static char template[] ="/tmp/myfileXXXXXX";
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
  char *dir = strdup(path); dir[2] = '\0';
  if (!file_exists(dir)) {
    char command[256];
    sprintf(command,"mkdir %s",dir);
    system(command);
  }
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
  char fname[100] = "/tmp/myfileXXXXXX";
  mkstemp(fname); wttf(wt,fname);

  if (stat(sha256file(fname),&st)==-1) 
    mkdir(sha256file(fname),0700);
  
  char *path = hashToPath(sha256file(fname));
  strcat(path,".t");
  cp(path,fname);
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



/* Part 3 - GESTION DES COMMITS */

/* Fonction de base */
char* blobCommit(Commit* c) {
  char fname[100] = "/tmp/myfileXXXXXX";
  mkstemp(fname); ctf(c,fname);

  if (stat(sha256file(fname),&st)==-1) 
    mkdir(sha256file(fname),0700);
  
  char *path = hashToPath(sha256file(fname));
  strcat(path,".c");
  cp(path,fname);
  return sha256file(fname);
}

/* MANIPULATION DES REFERENCES */
void initRefs(void) {
  if (!file_exists(".refs")) {
    system("mkdir .refs");
    system("touch .refs/master");
    system("touch .refs/HEAD");
  }
  return;
}
void createUpdateRef(char* ref_name,char* hash) {
  char buff[256];
  sprintf(buff,"echo %s > .refs/%s",hash,ref_name);
  system(buff);
}
void deleteRef(char* ref_name) {
  char buff[256];
  sprintf(buff,".refs/%s",ref_name);
  if (!file_exists(buff)) 
    printf("deletedRef: The reference %s does not exists\n",buff);
  else {
    sprintf(buff,"rm %s",buff);
    system(buff);
  }
  return;
}
char* getRef(char* ref_name) {
  char buff[256]; sprintf(buff,".refs/%s",ref_name);
  if (!file_exists(buff)) 
    printf("getRef: The reference %s does not exists\n",buff);
  FILE* f = fopen(buff,"r");
  char* res = (char*)malloc(MAX_INPUT);
  fgets(res,MAX_INPUT,f); fclose(f);
  return res;
}

/* SIMULATION */
void myGitAdd(char* file_or_folder) {
  WorkTree* wt;
  if (!file_exists(".add")) {
    system("touch .add");
    wt = initWorkTree();
  } else 
    wt = ftwt(".add");

  if (file_exists(file_or_folder)) {
    appendWorkTree(wt,file_or_folder,NULL,0);
    wttf(wt,".add");
  }
  else printf("git add: File or folder %s does not exists",file_or_folder);
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
  char *head = getRef("HEAD");
  char  *branch = getRef(branch_name);

  if (strcmp(head,branch)!=0) {
    printf("getCommit: HEAD doit pointer sur le dernier commit de la branche\n");
    return;
  }
  WorkTree* wt = ftwt(".add"); 
  char* h = saveWorkTree(wt,".");
  Commit* c = createCommit(h);
  if (strlen(branch)!=0) 
    commitSet(c,"predecessor",branch);
  if (message!=NULL) 
    commitSet(c,"message",message);
  char* hc = blobCommit(c);
  createUpdateRef(branch,hc);
  createUpdateRef("HEAD",hc);
  system("rm .add"); 
  free(branch); free(head);
  return;
}



/* Part 4 - GESTION D'UNE TIMELINE ARBORESCENTE */
/* Side function */
char* hashToPathCommit(char* hash) {
  char* buff = (char*)malloc(PATH_MAX);
  sprintf(buff,"%s.c",hashToPath(hash));
  return buff;
}

/* SIMULATION DE GIT CHECKOUT */
void restoreCommit(char* hash_commit) {
  Commit* c = ftc(hashToPathCommit(hash_commit));
  char* tree_hash = strcat(hashToPath(commitGet(c,"tree")),".t");
  restoreWorkTree(ftwt(tree_hash),".");
  return;
}
void myGitCheckoutBranch(char* branch) {
  char buff[PATH_MAX];
  sprintf(buff,"echo %s > .current_branch",branch);
  system(buff);

  char* hash_commit = getRef(branch);
  createUpdateRef("HEAD",hash_commit);
  restoreCommit(hash_commit);
  return;
}
