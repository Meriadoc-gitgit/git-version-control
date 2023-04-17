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
    if (file_exists(cp_path)) { //si le chemin hache existe
      if (isWorkTree(wt->tab[i].hash)==1) { //si c'est un repertoire
        mkdir(a_path,0700); //créer les repertoires s'il n'existe pas déjà
        strcat(cp_path,".t");
        WorkTree* newWT = ftwt(cp_path);
        restoreWorkTree(newWT,a_path);
        setMode(getChmod(cp_path),a_path);
      } else if (isWorkTree(wt->tab[i].hash)==0) { //si c'est un fichier
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
  char buff[100];
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
  //Usage: transformer le hash en entree en un path a un enregistrement d'un Commit
  char* buff = (char*)malloc(INT_MAX*sizeof(char)); //allouer cette variable pour enregistrer le path suivant
  strcat(buff,hashToPath(hash));
  strcat(buff,".c"); //concatener le path avec son extension
  return buff;
}

/* SIMULATION DE GIT CHECKOUT */
void restoreCommit(char* hash_commit) {
  //Usage: restaurer le WorkTree associé à un commit dont le hash est donnée en paramètre
  Commit* c = ftc(hashToPathCommit(hash_commit));
  char* tree_hash = strcat(hashToPath(commitGet(c,"tree")),".t");
  restoreWorkTree(ftwt(tree_hash),"."); //restaurer le WorkTree associe
  return;
}
void myGitCheckoutBranch(char* branch) {//ok
  //Usage: restaure le WorkTree correspondant au dernier commit de branch
  char buff[PATH_MAX*sizeof(char)]; //allouer cette variable pour l'enregistrement d'une commande
  sprintf(buff,"echo %s > .current_branch",branch); //modifier la direction le .current_branch
  system(buff);

  char* hash_commit = getRef(branch);
  createUpdateRef("HEAD",hash_commit); //mettre a jour la reference HEAD
  restoreCommit(hash_commit); //restaurer le Commit depuis le hash enregistre dans la branche en entree
  return;
}
List* filterList(List* L,char* pattern) {
  //Usage: retourne une nouvelle liste contenant uniquement les éléments de L commencant par pattern
  List* filtered = initList(); //initialiser une List
  for (Cell* ptr=*L;ptr!=NULL;ptr=ptr->next) { //parcourir la List fournie en entree
    char* c = strdup(ptr->data);
    c[strlen(pattern)] = '\0'; //indiquer la fin d'une chaine avec '\0'
    if (!strcmp(c,pattern)) //si ces 2 chaines se ressemblent
      insertFirst(filtered,buildCell(ptr->data)); //alors inserer ptr->data dans la List recemment cree
  }
  return filtered;
}
void myGitCheckoutCommit(char* pattern) {
  //Usage: filtre cette liste pour ne garder que ceux qui commencent par pattern
  List* filtered = filterList(getAllCommits(),pattern); //filtre la List de tous les Commit existants qui correspond a la chaine pattern
  if (sizeList(filtered)==1) { //s'il ne reste plus qu'un hash
    char* c_hash = listGet(filtered,0)->data;
    createUpdateRef("HEAD",c_hash); //met à jour la référence HEAD pour pointer sur ce hash
    restoreCommit(c_hash); //restaure le WorkTree correspondant
  } else {
    if (!sizeList(filtered)) //s'il ne reste plus aucun hash: affiche le message d'erreur à l'utilisateur
      printf("WARNING: No pattern matching.\n");
    else {
      printf("WARNING: Multiple matchings found:\n");
      for (Cell* ptr=*filtered;ptr!=NULL;ptr=ptr->next)
        printf("-> %s\n",ptr->data);
      //s'il reste plusieurs hash: affiche tous + demande à l'utilisateur de préciser sa requête
    }
  }
  return;
}



/* Part 5+6 - FINAL */
/* SIMULATION DE GIT MERGE */
WorkTree* mergeWorkTree(WorkTree* wt1, WorkTree* wt2, List** conflicts) {//ok
  /*Usage: crée:
  1. une liste de chaines de caractères composée des noms de fichiers/répertoires qui sont en conflit
  2. un nouveau WorkTree des fichiers et/ou répertoires qui ne sont pas en conflit*/
  for (int i=0;i<wt1->n;i++) {
    if (inWorkTree(wt2,wt1->tab[i].name))
      insertFirst(*conflicts,buildCell(wt1->tab[i].name)); //creer la List des conflits si 1 nom de fichier existe dans tous les 2 WorkTree en entree
  }
  WorkTree* wt = initWorkTree(); //initialiser un WorkTree
  for (int i=0;i<wt1->n;i++) {
    if (!searchList(*conflicts,wt1->tab[i].name))
      appendWorkTree(wt,wt1->tab[i].name,wt1->tab[i].hash,wt1->tab[i].mode); //inserer le fichier du premier WorkTree dans le nouveau WorkTree ssi il n'existe pas dans la List des conflits
  }
  for (int i=0;i<wt2->n;i++) {
    if (!searchList(*conflicts,wt2->tab[i].name))
      appendWorkTree(wt,wt2->tab[i].name,wt2->tab[i].hash,wt2->tab[i].mode); //inserer le fichier du deuxieme WorkTree dans le nouveau WorkTree ssi il n'existe pas dans la List des conflits
  }
  return wt;
}
List* merge(char* remote_branch, char* message) {
  //Usage: Fusionne la branche courante avec la branche passée en paramètre si aucun conflit n'existe.
  char* current_commit_h = getRef(getCurrentBranch());
  char* remote_commit_h = getRef(remote_branch);

  /*Initialiser 2 Commits pour faciliter les opérations*/
  Commit* c1 = ftc(hashToPathCommit(current_commit_h));
  Commit* c2 = ftc(hashToPathCommit(remote_commit_h));

  /*Initialiser 2 paths afin de faciliter les opérations*/
  char* chemin1 = strcat(hashToPath(commitGet(c1,"tree")),".t");
  char* chemin2 = strcat(hashToPath(commitGet(c2,"tree")),".t");

  //Initialiser 2 WorkTree associes a la variable remote_branch et a la branche courante
  WorkTree* current = ftwt(chemin1);
  WorkTree* remote = ftwt(chemin2);

  List* conflicts = initList(); //initialiser une List qui va enregistrer les conflits

  WorkTree* wt = mergeWorkTree(current,remote,&conflicts); //crée le worktree de fusion (avec mergeWorkTree)

  /*
  Si au moins un conflit:
  - aucune de ses opérations n'est effectuée
  - la fonction retourne la liste des conflits
  */
  if (conflicts) return conflicts;
  
  //Si aucun conflit
  Commit* c = createCommit(blobWorkTree(wt)); //crée le commit associé à ce nouveau worktree
  commitSet(c,"predecessor",current_commit_h); //..en indiquant qui sont des prédécesseurs
  commitSet(c,"merged_predecessor",remote_commit_h); //..en lui ajoutant le message descriptif passé en paramètre
  char* c_h = blobCommit(c); //réalise un enregistrement instantané du worktree de fusion + de ce nouveau commit
  myGitCommit(getCurrentBranch(),message); //ajouter le nouveau commit à la branche courante
  createUpdateRef("HEAD",c_h); //MAJ la référence de la branche courante et la référence HEAD pour pointer vers ce nouveau commit
  deleteRef(remote_branch); //supprimer la référence de la branche passée en paramètre
  restoreWorkTree(wt,"."); //restaurer le projet correspondant au worktree de fusion
  return NULL;
}
void createDeletionCommit(char* branch, List* conflicts, char* message) {
  //Usage: crée + ajoute un commit de suppression sur branch, correspondant à la suppression des éléments de la liste conflicts.
  char* depart = getCurrentBranch();
  myGitCheckoutBranch(branch); //se déplacer sur la branche en question (à l'aide de myGitCheckoutBranch)
  Commit* c = ftc(hashToPathCommit(getRef(branch))); //récupère le dernier commit de cette branche + le worktree associé
  WorkTree* wt = ftwt(strcat(hashToPath(commitGet(c,"tree")),".t"));
  system("echo > .add"); //vider la zone de preparation
  for(int i=0;i<wt->n;i++) {
    if (!searchList(conflicts,wt->tab[i].name)) 
      myGitAdd(wt->tab[i].name); //utiliser myGitAdd pour ajouter les fichiers/répertoires du worktree qui ne font pas partie de la liste des conflits
  }
  myGitCommit(branch,message); //appelle myGitCommit pour créer le commit de suppression
  myGitCheckoutBranch(depart); //revient sur la branche de départ (à l'aide de myGitCheckoutBranch)
  return;
}