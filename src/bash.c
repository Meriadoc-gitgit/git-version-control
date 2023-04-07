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
  /* Creation de fichier temporaire */
  static char template[] ="/tmp/myfileXXXXXX";
  char fname[1000];
  strcpy(fname,template);
  mkstemp(fname);

  /* Enregistrement du hash du fichier en parametre au fichier temporaore */
  hashFile(file,fname);
  /* Recuperation du hash du fichier en parametre enregistre dans le fichier temporaire */
  char *hash = (char*)malloc(256*sizeof(char));
  FILE *f = fopen(fname,"r");
  fgets(hash,256,f);
  fclose(f);

  /* Commande envoye au systeme pour supprimer le fichier temporaire interne */
  char cmd[10000]; 
  sprintf(cmd,"rm %s",fname);
  system(cmd);
  return hash;
}


List* listdir(char* root_dir) {
  List* L = initList();
  /* Parcourir la liste des fichiers et/ou repertoires contenant dans root_dir */
  DIR* dp = opendir(root_dir);
  struct dirent* ep;
  if (dp) {
    while((ep = readdir(dp))) {
      /* Inserer le nom du contenu de root_dir dans une List L */
      insertFirst(L,buildCell(ep->d_name));
      List ptr = *L;
    }
    (void)closedir(dp); //fermer le flux apres l'usage
  } else {
    /* Au cas ou parcours echoue, rattrapper le message d'erreur */
    perror("listdir: Could not open the directory\n");
    return NULL;
  }
  return L;
}
int file_exists(char* file) {
  struct stat buffer; //structure that provides detailed information about a file
  return (stat(file,&buffer)==0);
}
void cp(char* to, char* from) {
  if(file_exists(from)) {
    char ligne[256];
    FILE* ffrom = fopen(from,"r"); FILE* fto = fopen(to,"w");
    while(fgets(ligne,256,ffrom)) //copie par lecture ligne par ligne du fichier source
      fputs(ligne,fto);
    fclose(ffrom); fclose(fto);
  }
  return;
}
char* hashToPath(char* hash) {
  char* dir = (char*)malloc((strlen(hash)+1)*sizeof(char));
  dir[0] = hash[0]; dir[1] = hash[1]; dir[2] = '/'; //copier la repertoire du hash
  int i;
  for(i=3;i<=strlen(hash);i++)
    dir[i] = hash[i-1]; //copier chaque caractere du hash apres l'ajout de '/'
  dir[i] = '\0'; // caractere fin 
  return dir;
}
void blobFile(char* file) { //ok, mais a re-tester
  if (!file_exists(file)) {
    /* verifier l'existence du fichier en parametre dans le repertoire courant */
    printf("blobFile: Fichier demande n'existe pas\n");
    return;
  }
  char *path = hashToPath(sha256file(file));
  char *dir = strdup(path); dir[2] = '\0'; //extraire le repertoire du hash
  if (!file_exists(dir)) {
    char command[256]; 
    /* creer le repertoire extrait s'il n'existe pas deja */
    sprintf(command,"mkdir %s",dir);
    system(command);
  }
  if (file_exists(path)) return; //si le fichier hash existe deja, aucune operation de plus
  cp(path,file); //copier le contenu du fichier en parametre dans le path extrait de son hash
  return;
}



/* Part 2 */
/* Fonction fournie pour la manipulation des chmod */
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
char* hashToFile(char* hash) {
  char* ch = strdup(hash);
  ch[2] = '\0'; //extrait de nom de repertoire du hash
  if (stat(ch,&st)==-1) //creer le repertoire s'il n'existe pas deja
    mkdir(ch,0700);
  return hashToPath(hash);
}
char* blobWorkTree(WorkTree* wt) {
  /* creation du fichier temporaire */
  char fname[100] = "/tmp/myfileXXXXXX";
  mkstemp(fname); wttf(wt,fname); //ecrire le contenu de worktree dans le fichier temporaire

  char* hash = sha256file(fname);
  char* ch = hashToFile(hash);
  strcat(ch,".t"); //ajouter l'extension '.t' pour se differer le repertoire des fichiers
  cp(ch,fname); //copier le contenu du fichier temporaire au path extrait du hash
  return hash;
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
    if (listdir(a_path)) { //si c'est un repertoire
      List* L = listdir(WF.name);
      WorkTree* newWT = initWorkTree(); 
      while(*L) {
        //creer le nouveau worktree pour l'appel recursive de cette fonction
        appendWorkTree(newWT,(*L)->data,sha256file((*L)->data),getChmod((*L)->data));
        *L = (*L)->next;
      }
      WF.hash = saveWorkTree(newWT,a_path);
      WF.mode = getChmod(a_path);
    } else { //si c'est un simple fichier
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
      //si c'est un repertoire
      // strstr(arg1,arg2): trouver la sous-chaine similaire indique dans args2 de arg1
      strcat(a_path,".t");
      WorkTree* newWT = ftwt(cpPath);
      restoreWorkTree(newWT,a_path);
      setMode(getChmod(cpPath),a_path);
    } else { //si c'est un fichier
      cp(a_path,cpPath); 
      setMode(getChmod(cpPath),a_path);
    }
  }
  return;
}



/* Part 3 - GESTION DES COMMITS */
/* Fonction de base */
char* blobCommit(Commit* c) {
  /* Meme procedure que blobWorkTree en utilisant l'extension .c au lieu de .t */
  char fname[100] = "/tmp/myfileXXXXXX";
  mkstemp(fname); ctf(c,fname);

  char* hash = sha256file(fname);
  char* ch = hashToFile(hash);
  strcat(ch,".c");
  cp(ch,fname);
  return hash;
}

/* MANIPULATION DES REFERENCES */
void initRefs(void) {
  if (!file_exists(".refs")) { //si .refs n'existe pas deja
    system("mkdir .refs");
    system("touch .refs/master");
    system("touch .refs/HEAD");
  }
  return;
}
void createUpdateRef(char* ref_name,char* hash) {
  char buff[256];
  /* echo hash > .refs/ref_name: ecrire hash dans .refs/ref_name */
  sprintf(buff,"echo %s > .refs/%s",hash,ref_name);
  system(buff);
  return;
}
void deleteRef(char* ref_name) {//ok
  char buff[256];
  sprintf(buff,".refs/%s",ref_name);
  if (!file_exists(buff)) //verifier l'existence de buff dans .refs
    printf("deletedRef: The reference %s does not exists\n",buff);
  else {
    sprintf(buff,"rm .refs/%s",ref_name);
    system(buff);
  }
  return;
}
char* getRef(char* ref_name) {
  char buff[256]; sprintf(buff,".refs/%s",ref_name);
  if (!file_exists(buff)) //verifier l'existence de ref_name dans .refs
    printf("getRef: The reference %s does not exists\n",buff);
  FILE* f = fopen(buff,"r");
  char* res = (char*)malloc(MAX_INPUT);
  fgets(res,MAX_INPUT,f); fclose(f); //lire le contenu de ref_name + l'enregistrer dans res
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
    printf("git commit: La branche n'existe pas\n");
    return;
  }
  char *head = getRef("HEAD");
  char  *branch = getRef(branch_name);

  if (strcmp(head,branch)!=0) {
    printf("git commit: HEAD doit pointer sur le dernier commit de la branche\n");
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
  free(tree_hash);
  return;
}
void myGitCheckoutBranch(char* branch) {
  char buff[PATH_MAX];
  sprintf(buff,"echo %s > .current_branch",branch);
  system(buff);

  char* hash_commit = getRef(branch);
  createUpdateRef("HEAD",hash_commit);
  restoreCommit(hash_commit);
  free(hash_commit);
  return;
}
List* filterList(List* L,char* pattern) {
  List* filtered = initList();
  for (Cell* ptr=*L;ptr!=NULL;ptr=ptr->next) {
    char* c = strdup(ptr->data);
    c[strlen(pattern)] = '\0';
    if (!strcmp(c,pattern))
      insertFirst(filtered,buildCell(c));
    free(c);
  }
  return filtered;
}
void myGitCheckoutCommit(char* pattern) {
  List* filtered = filterList(getAllCommits(),pattern);
  if (sizeList(filtered)==1) {
    char* c_hash = listGet(filtered,0)->data;
    createUpdateRef("HEAD",c_hash);
    restoreCommit(c_hash);
    free(c_hash);
  } else {
    if (!sizeList(filtered)) 
      printf("No pattern matching.\n");
    else {
      printf("Multiple matchings found:\n");
      for (Cell* ptr=*filtered;ptr!=NULL;ptr=ptr->next)
        printf("-> %s\n",ptr->data);
    }
  }
  return;
}



/* Part 5+6 - FINAL */
/* SIMULATION DE GIT MERGE */
WorkTree* mergeWorkTree(WorkTree* wt1, WorkTree* wt2, List** conflicts) {
  for (int i=0;i<wt1->n;i++) {
    if (inWorkTree(wt2,wt1->tab[i].name))
      insertFirst(*conflicts,buildCell(wt1->tab[i].name));
  }
  WorkTree* wt = initWorkTree();
  for (int i=0;i<wt1->n;i++) {
    if (!searchList(*conflicts,wt1->tab[i].name))
      appendWorkTree(wt,wt1->tab[i].name,wt1->tab[i].hash,wt1->tab[i].mode);
  }
  for (int i=0;i<wt2->n;i++) {
    if (!searchList(*conflicts,wt2->tab[i].name))
      appendWorkTree(wt,wt2->tab[i].name,wt2->tab[i].hash,wt2->tab[i].mode);
  }
  return wt;
}
List* merge(char* remote_branch, char* message) {
  char* current_commit_h = getRef(getCurrentBranch());
  char* remote_commit_h = getRef(remote_branch);

  WorkTree* current = ftwt(strcat(hashToPath(commitGet(ftc(hashToPathCommit(current_commit_h)),"tree")),".t"));
  WorkTree* remote = ftwt(strcat(hashToPath(commitGet(ftc(hashToPathCommit(remote_commit_h)),"tree")),".t"));

  List* conflicts = initList();
  if (conflicts) return conflicts;

  WorkTree* wt = mergeWorkTree(current,remote,&conflicts);
  
  Commit* c = createCommit(blobWorkTree(wt));
  commitSet(c,"predecessor",current_commit_h);
  commitSet(c,"merged_predecessor",remote_commit_h);
  char* c_h = blobCommit(c);
  myGitCommit(getCurrentBranch(),message);
  createUpdateRef("HEAD",c_h);
  deleteRef(remote_branch);
  restoreWorkTree(wt,".");

  free(current_commit_h); free(remote_commit_h); free(c_h);
  libererWorkTree(current); libererWorkTree(remote);
  return NULL;
}
void createDeletionCommit(char* branch, List* conflicts, char* message) {
  char* depart = getCurrentBranch();
  myGitCheckoutBranch(branch);
  Commit* c = ftc(hashToPathCommit(getRef(getCurrentBranch())));
  WorkTree* wt = ftwt(strcat(hashToPath(commitGet(c,"tree")),".t"));
  system("echo > .add"); // vider la zone de preparation
  for(int i=0;i<wt->n;i++) {
    if (!searchList(conflicts,wt->tab[i].name)) 
      myGitAdd(wt->tab[i].name);
  }
  myGitCommit(branch,message);
  myGitCheckoutBranch(depart); free(depart); libererWorkTree(wt);
  return;
}