#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>

#include "src.h"
/* SIDE FUNCTION */
int isFile(const char* fileName) {
  //Usage: verifier si fileName est un fichier. 
  //Retour: 1 si c'est un fichier, 0 sinon
  struct stat path; //declaration de structure stat
  stat(fileName, &path);
  return S_ISREG(path.st_mode);
}

/* Part 1 */
int hashFile(char* src, char *dst) {
  //Usage: calcule le hash du contenu du premier fichier + l'écrit dans le deuxième fichier
  char buff[100];
  sprintf(buff,"cat %s | sha256sum > %s",src,dst);
  return system(buff);
}

char* sha256file(char* file) {
  //Usage: renvoie 1 chaine de caractères contenant le hash du fichier donné en paramètre
  if (isFile(file)==0) return NULL; //si file est un repertoire
  
  /* Creation de fichier temporaire */
  static char template[] ="/tmp/myfileXXXXXX";
  char fname[1000];
  strcpy(fname,template);
  mkstemp(fname);

  /* Enregistrement du hash du fichier en parametre au fichier temporaore */
  hashFile(file,fname);
  /* Recuperation du hash du fichier en parametre enregistre dans le fichier temporaire */
  char *hash = (char*)malloc(256*sizeof(char)); //allouer cette variable pour l'enregistrement du contenu du fichier temporaire
  FILE *f = fopen(fname,"r"); //ouverture de fichier temporaire fname ainsi cree
  fgets(hash,256,f); //enregistrer son contenu dans hash
  fclose(f); //fermer le fichier temporaire

  /* Commande envoye au systeme pour supprimer le fichier temporaire interne */
  char cmd[10000]; 
  sprintf(cmd,"rm %s",fname);
  system(cmd); //supprimer le fichier temporaire du systeme
  return hash;
}


List* listdir(char* root_dir) {
  //Usage: renvoie une liste contenant les noms des fichiers et répertoires qui s'y trouvent
  List* L = initList(); //initialise une List
  /* Parcourir la liste des fichiers et/ou repertoires contenant dans root_dir */
  DIR* dp = opendir(root_dir);
  struct dirent* ep;
  if (dp) {
    while((ep = readdir(dp))) {
      insertFirst(L,buildCell(ep->d_name)); // Inserer le nom du contenu de root_dir dans une List L
    }
    (void)closedir(dp); //fermer le flux apres l'usage
  } else {
    /* Au cas ou parcours echoue, rattrapper le message d'erreur */
    perror("WARNING: Could not open the directory\n");
    return NULL;
  }
  return L;
}
int file_exists(char* file) {
  //Usage: renvoie 1 si le fichier existe dans le répertoire courant et 0 sinon
  struct stat buffer; //structure that provides detailed information about a file
  return (stat(file,&buffer)==0);
}
void cp(char* to, char* from) {
  //Usage: copie le contenu d'un fichier vers un autre (en lisant ligne par ligne du fichier source)
  if(file_exists(from)) { //verifier l'existence du fichier source dans le systeme
    char ligne[256]; //alouer cette variable pour faire la lecture ligne par ligne suivant
    FILE* ffrom = fopen(from,"r"); 
    FILE* fto = fopen(to,"w");
    while(fgets(ligne,256,ffrom)) //copie par lecture ligne par ligne du fichier source
      fprintf(fto,"%s",ligne);
    fclose(ffrom); 
    fclose(fto); //fermer tous les 2 fichiers
  }
  return;
}
char* hashToPath(char* hash) {
  //Usage: retourne le chemin d'un fichier à partir de son hash
  char* dir = (char*)malloc((strlen(hash)+1)*sizeof(char));
  dir[0] = hash[0]; 
  dir[1] = hash[1]; //copier la repertoire du hash
  dir[2] = '/'; //ajouter le slash '/' pour distinguer le nom du repertoire et celui du fichier
  int i;
  for(i=3;i<=strlen(hash);i++)
    dir[i] = hash[i-1]; //copier chaque caractere du hash apres l'ajout de '/'
  dir[i] = '\0'; // caractere fin 
  return dir;
}
void blobFile(char* file) { 
  //Usage: enregistre un instantané (réalise un sauvegarde) du fichier donnée en entrée
  if (!file_exists(file)) {
    /* verifier l'existence du fichier en parametre dans le repertoire courant */
    printf("WARNING: %s does not exist\n",file);
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
  mkstemp(fname); 
  wttf(wt,fname); //ecrire le contenu de worktree dans le fichier temporaire

  char* hash = sha256file(fname);
  char* ch = hashToFile(hash);
  strcat(ch,".t"); //ajouter l'extension '.t' pour se differer le repertoire des fichiers
  //printf("ch: %s\n",ch);
  cp(ch,fname); //copier le contenu du fichier temporaire au path extrait du hash
  return hash;
}
char* concat(char* s1,char* s2) {
  //Usage: concatener 2 chaines de caracteres pour former un path a un fichier/repertoire
  char* dir = (char*)malloc(INT_MAX*sizeof(char));
  strcat(dir,s1); 
  strcat(dir,"/"); 
  strcat(dir,s2);
  return dir;
}
char* saveWorkTree(WorkTree* wt,char* path) {//ok
  for (int i=0;i<wt->n;i++) {
    char* a_path = concat(path,wt->tab[i].name);
    if (!file_exists(hashToPath(wt->tab[i].hash)) && wt->tab[i].name[0]!='.') {
      if (isFile(a_path)==0) { //si c'est un repertoire
        //printf("rep %s\n",wt->tab[i].name);
        List* L = listdir(a_path);
        WorkTree* newWT = initWorkTree(); 
        while(*L) {
          //creer le nouveau worktree pour l'appel recursive de cette fonction
          appendWorkTree(newWT,(*L)->data,sha256file((*L)->data),getChmod((*L)->data));
          *L = (*L)->next;
        }
        wt->tab[i].hash = saveWorkTree(newWT,a_path); //appel recursive de saveWT
        wt->tab[i].mode = getChmod(a_path);
      } else { //si c'est un simple fichier
        //printf("fic %s\n",wt->tab[i].name);
        if (file_exists(wt->tab[i].name)) {
          /*enregistrer ssi ce fichier exists!! afin d'éviter l'enregistrement des hash*/
          blobFile(wt->tab[i].name); //enregistrer un fichier de WorkTree
          wt->tab[i].hash = sha256file(wt->tab[i].name);
          wt->tab[i].mode = getChmod(a_path);
        }
      }
    }
  }
  return blobWorkTree(wt);
}
int isWorkTree(char* hash) {
  //Verifier si hash est un WorkTree
  if (file_exists(strcat(hashToPath(hash),".t"))) return 1;
  if (file_exists(hashToPath(hash))) return 0;
  return -1; //en cas de ni WorkTree ni WorkFile
}
void restoreWorkTree(WorkTree* wt,char* path) {//ok
  /*Nécessaire de vider le path avant de restaurer un WorkTree*/
  char command[PATH_MAX];
  sprintf(command,"rm -f %s/*",path); //vider le path avant la restauration (afin d'eviter les fautes de segmentation)
  system(command);
  /* Parcours principal */
  for (int i=0;i<wt->n;i++) {
    char* a_path = concat(path,wt->tab[i].name);
    char* cp_path = hashToPath(wt->tab[i].hash);
    //printf("status %s: %d\n",wt->tab[i].name,isWorkTree(wt->tab[i].hash));
    if (file_exists(cp_path)) { //si le chemin hache existe
      if (isWorkTree(wt->tab[i].hash)==1) { //si c'est un repertoire
        //printf("rep %s\n",wt->tab[i].name);
        strcat(cp_path,".t");
        WorkTree* newWT = ftwt(cp_path);
        mkdir(a_path,0700); //new line, need to  retest, remove if not working
        restoreWorkTree(newWT,a_path);
        setMode(getChmod(cp_path),a_path);
      } else if (isWorkTree(wt->tab[i].hash)==0) { //si c'est un fichier
        //printf("fic %s\n",wt->tab[i].name);
        cp(a_path,cp_path); 
        setMode(getChmod(cp_path),a_path);
      }
    }
  }
  return;
}



/* Part 3 - GESTION DES COMMITS */
/* Fonction de base */
char* blobCommit(Commit* c) {
  /* Meme procedure que blobWorkTree en utilisant l'extension .c au lieu de .t */
  char fname[100] = "/tmp/myfileXXXXXX";
  mkstemp(fname); 
  ctf(c,fname);

  char* hash = sha256file(fname);
  char* ch = hashToFile(hash);
  strcat(ch,".c");
  cp(ch,fname);
  return hash;
}

/* MANIPULATION DES REFERENCES */
void initRefs(void) {
  //Usage: crée le répertoire caché .refs (s'il n'existe pas déjà) + crée master et HEAD
  if (!file_exists(".refs")) { //si .refs n'existe pas deja
    system("mkdir .refs");
    system("touch .refs/master"); //creer la reference master
    system("touch .refs/HEAD"); //creer la reference HEAD
  }
  return;
}
void createUpdateRef(char* ref_name,char* hash) { 
  //Usage: met à jour une référence en remplacent son contenu par hash
  char buff[256];
  sprintf(buff,".refs/%s",ref_name);
  if (!file_exists(buff)) { //Si la référence n'existe pas: la fonction commence par créer le fichier
    char cmd[256];
    sprintf(cmd,"touch %s",buff);
    system(cmd);
  }
  FILE* f = fopen(buff,"w");
  fprintf(f,"%s",hash); //enregistrer le hash fourni en entree dans le fichier 
  fclose(f);
  return;
}
void deleteRef(char* ref_name) {//ok
  //Usage: supprime une référence
  char buff[256];
  sprintf(buff,".refs/%s",ref_name);
  if (!file_exists(buff)) //verifier l'existence de buff dans .refs
    printf("WARNING: The reference %s does not exists\n",buff);
  else { //si la reference existe dans le systeme, on effectue l'instruction de suppression
    sprintf(buff,"rm .refs/%s",ref_name);
    system(buff);
  }
  return;
}
char* getRef(char* ref_name) {
  //Usage: récupère vers quoi pointe une référence (le hash contenu dans le fichier)
  char buff[256];
  sprintf(buff,".refs/%s",ref_name);
  if (!file_exists(buff)) return NULL; //Si le fichier n'existe pas, retourne NULL
  FILE* f = fopen(buff,"r");
  char* res = (char*)malloc(INT_MAX);
  fgets(res,INT_MAX,f); //lire le contenu de ref_name + l'enregistrer dans res
  fclose(f); 
  return res; //possible d'etre vide si le fichier est vide
}

/* SIMULATION */
void myGitAdd(char* file_or_folder) {
  //Usage: ajouter un fichier ou répertoire dans le WorkTree correspondant à la zone de préparation
  WorkTree* wt;
  if (!file_exists(".add")) { //Si .add n'existe pas, il faut d'abord le créer
    system("touch .add");
    wt = initWorkTree();
  } else 
    wt = ftwt(".add"); //initialiser un WorkTree depuis .add

  if (file_exists(file_or_folder)) { //si le fichier en entree existe deja dans le systeme
    appendWorkTree(wt,file_or_folder,NULL,0); //ajouter 1 WorkFile + mettre a jour .add
    wttf(wt,".add");
  }
  else printf("WARNING: File or folder %s does not exists",file_or_folder);
  return;
}
void myGitCommit(char* branch_name,char* message) {//ok
  if (!file_exists(".refs")) { //si le repertoire .refs n'existe pas deja
    printf("WARNING: Require to initialize project references first\n");
    return;
  }
  char* path = concat(".refs",branch_name);

  if (!file_exists(path)) { //si la branche n'existe pas deja
    printf("WARNING: The branch does not exist\n");
    return;
  }
  char *head = getRef("HEAD");
  char *branch = getRef(branch_name);

  if (strcmp(head,branch)!=0) { //si le HEAD et la branche fournie ne pointe pas sur le meme Commit
    printf("WARNING: HEAD must point to the last commit of the branch\n");
    return;
  }
  //Si toutes les tests suivantes sont passees
  WorkTree* wt = ftwt(".add"); 
  char* h = saveWorkTree(wt,"."); //sauvegarder le WorkTree de la zone de preparation
  Commit* c = createCommit(h); 
  if (strlen(branch)!=0) //si la branche n'est pas vide
    commitSet(c,"predecessor",branch);

  if (message!=NULL) //s'il y a une message de description pour le Commit
    commitSet(c,"message",message);

  char* hc = blobCommit(c); //enregistrer le Commit
  createUpdateRef(branch,hc); //mettre a jour la branche
  createUpdateRef("HEAD",hc); //mettre a jour HEAD
  system("rm .add"); //enlever .add du systeme apres avoir tout Commit les fichiers de la zone de preparation
  return;
}



/* Part 4 - GESTION D'UNE TIMELINE ARBORESCENTE */
/* Side function */
char* hashToPathCommit(char* hash) {
  char* buff = (char*)malloc(INT_MAX*sizeof(char));
  strcat(buff,hashToPath(hash));
  strcat(buff,".c");
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
void myGitCheckoutBranch(char* branch) {//ok
  char buff[PATH_MAX];
  sprintf(buff,"echo %s > .current_branch",branch);
  system(buff);

  char* hash_commit = getRef(branch);
  createUpdateRef("HEAD",hash_commit);
  restoreCommit(hash_commit);
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
WorkTree* mergeWorkTree(WorkTree* wt1, WorkTree* wt2, List** conflicts) {//ok
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

  WorkTree* current = ftwt(strcat(
    hashToPath(commitGet(
      ftc(hashToPathCommit(current_commit_h)),"tree")),".t"));
  WorkTree* remote = ftwt(strcat(
    hashToPath(commitGet(
      ftc(hashToPathCommit(remote_commit_h)),"tree")),".t"));

  List* conflicts = initList();

  WorkTree* wt = mergeWorkTree(current,remote,&conflicts);
  if (conflicts) return conflicts;
  
  Commit* c = createCommit(blobWorkTree(wt));
  commitSet(c,"predecessor",current_commit_h);
  commitSet(c,"merged_predecessor",remote_commit_h);
  char* c_h = blobCommit(c);
  myGitCommit(getCurrentBranch(),message);
  createUpdateRef("HEAD",c_h);
  deleteRef(remote_branch);
  restoreWorkTree(wt,".");
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
  myGitCheckoutBranch(depart); 
  free(depart); 
  return;
}