#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>

#include "bash.h"
#include "src.h"

/* Part 1 - Manipulation de Cell et List */
int sizeList(List* L) {
  int cpt = 0;
  while (*L) {
    cpt++; 
    *L = (*L)->next;
  }
  return cpt;
}
List* initList() { //OK
  List* rst = (List*)malloc(sizeof(List));
  *rst = NULL;
  return rst;
}
Cell* buildCell(char* ch) {//OK
  Cell* rst = (Cell*)malloc(sizeof(Cell));
  rst->data = strdup(ch);
  rst->next = NULL;
  return rst;
}
void insertFirst(List* L,Cell* C) {//OK
  C->next = *L; 
  *L = C;
  return;
}
char* ctos(Cell* C) {//OK
  if (C==NULL) return "";
  return C->data;
}
char* ltos(List* L) {//OK
  if (*L==NULL) 
    return "";  
  char* ch = malloc(INT_MAX*sizeof(char));
  while(*L) {
    strcat(ch,(*L)->data);
    *L = (*L)->next;
    if (*L) strcat(ch,"|");
  }
  return ch;
}
Cell* listGet(List* L,int i) {//OK
  if (*L==NULL) return NULL;
  Cell* tmp = *L;
  while(i>0) {
    tmp = tmp->next; 
    i--;
  }
  if (i==0) return tmp;
  else return NULL;
}
Cell* searchList(List* L,char* str) {//OK
  Cell* tmp = *L;
  while(tmp && strcmp(tmp->data,str)!=0) 
    tmp = tmp->next;
  return tmp;
}
List* stol(char* s) {//ok
  char str[(int)strlen(s)]; 
  sprintf(str,"%s",s);
  const char* delim = "|";
  char* ptr = strtok(str,delim); 
  List* L = initList();

  while (ptr!=NULL) {
    insertFirst(L,buildCell(ptr)); 
    ptr = strtok(NULL,delim); 
  }
  return L;
}
void ltof(List* L,char* path) {//ok
  if (*L==NULL) {
    printf("ltof: Liste *L est NULL\n");
    return;
  }
  FILE *f = fopen(path,"w");
  if (!f) {
    printf("ltof: Erreur lors de l'ouverture\n");
    return;
  }
  Cell *C = *L;
  while(C) {
    fprintf(f,"%s\n",C->data); 
    C = C->next;
  }
  fclose(f);
  return;
}
List* ftol(char* path) {//ok
  if (!file_exists(path)) {
    printf("ftol: Fichier introuvable\n");
    return NULL;
  }
  FILE *f = fopen(path,"r");
  if (!f) {
    printf("ftol: Erreur lors de l'ouverture\n");
    return NULL;
  }
  List* L = initList();
  char buffer[256]; 
  char *res = fgets(buffer, 256, f); 
  char data[26];
  while(res!=NULL) {
    sscanf(buffer,"%s",data);
    res = fgets(buffer, 256, f);
    insertFirst(L,buildCell(data));
  }
  fclose(f);
  return L;
}



/* Part 2 */
/* MANIPULATION DE WORKFILE */
WorkFile* createWorkFile(char* name) {//ok
  WorkFile* wf = (WorkFile*)malloc(sizeof(WorkFile));
  wf->name = strdup(name);
  wf->hash = NULL; 
  wf->mode = 0;
  return wf;
}
char* wfts(WorkFile* wf) {//ok
  if (!wf) {
    printf("Erreur wfts: WF NULL");
    return NULL;
  }
  char* command = (char*)malloc(1000*sizeof(char));
  sprintf(command,"%s\t%s\t%d",wf->name,wf->hash,wf->mode);
  return command;
}
WorkFile* stwf(char* ch) {//ok
  char str[(int)strlen(ch)]; 
  sprintf(str,"%s",ch);
  const char* delim = "\t";
  char name[100], hash[100]; 
  int mode;
  sscanf(ch,"%s %s %d",name,hash,&mode);
  WorkFile* wf = createWorkFile(name);
  wf->hash = strdup(hash);
  wf->mode = mode;
  return wf;
}
void libererWorkFile(WorkFile* wf) {//ok
  free(wf->name); 
  free(wf->hash); 
  free(wf);
  return;
}


WorkTree* initWorkTree() {//ok
  WorkTree* wt = (WorkTree*)malloc(sizeof(WorkTree));
  wt->size = MAX_INPUT; 
  wt->n = 0;
  wt->tab = (WorkFile*)malloc(wt->size*sizeof(WorkFile));
  return wt;
}
int inWorkTree(WorkTree* wt,char* name) {//ok
  if (!wt) {
    printf("inWorkTree: wt NULL\n"); return -1;
  }
  for (int i=0;i<wt->n;i++) {
    if (strcmp(wt->tab[i].name,name)==0) 
      return i;
  }
  return -1;
}
int appendWorkTree(WorkTree* wt,char* name,char* hash,int mode) {//ok
  if (inWorkTree(wt,name)==-1 && wt->n<wt->size) {
    wt->tab[wt->n].name = strdup(name);
    wt->tab[wt->n].mode = mode;
    if (hash) wt->tab[wt->n].hash = strdup(hash);
    else wt->tab[wt->n].hash = "";
    wt->n++;
    return 0;
  }
  return -1;
}
char* wtts(WorkTree* wt) {//ok
  if (wt->n==0) return "wtts: WorkTree vide\n";
  char *command = (char*)malloc(INT_MAX);
  char* w = NULL;
  for (int i=0;i<wt->n;i++) {
    w = wfts(&(wt->tab[i]));
    strcat(command,w); 
    strcat(command,"\n");
  }
  return command;
}
WorkTree* stwt(char* ch) {//ok
  int pos = 0; 
  int n_pos = 0; 
  int sep = '\n';
  char* ptr;
  char* result = malloc(sizeof(char)*10000);
  WorkTree* wt = initWorkTree();
  while (pos<strlen(ch)) {
    ptr = strchr(ch+pos,sep);
    if (ptr==NULL)
      n_pos = strlen(ch)+1;
    else 
      n_pos = ptr - ch + 1;
    
    memcpy(result, ch+pos, n_pos-pos-1);
    result[n_pos-pos-1] = '\0';
    pos = n_pos;
    WorkFile* wf = stwf(result);
    appendWorkTree(wt, wf->name, wf->hash, wf->mode);
  }
  return wt;
}
int wttf(WorkTree* wt,char* file) {//ok
  FILE* f = fopen(file,"w");
  if (!f) {
    printf("wttf: Erreur lors de l'ouverture de fichier\n"); 
    return -1;
  }
  fprintf(f,"%s",wtts(wt));
  fclose(f); 
  return 0;
}
WorkTree* ftwt(char* file) {//ok
  FILE* f = fopen(file,"r");
  if (!f) {
    printf("ftwt: Erreur lors de l'ouverture\n");
    return NULL;
  }
  WorkTree* wt = initWorkTree();
  char buffer[256]; 
  char* res = fgets(buffer,256,f); 
  char name[26], hash[256]; 
  int mode;
  while (res!=NULL) {
    sscanf(buffer,"%s\t%s\t%d",name,hash,&mode);
    appendWorkTree(wt,name,hash,mode);
    res = fgets(buffer,256,f);
  }
  fclose(f);
  return wt;
}
void libererWorkTree(WorkTree* wt) {//ok
  if (wt->n==0) return;
  for(int i=0;i<wt->n;i++){ 
    WorkFile *w=&(wt->tab[i]);
    free(w->name);
    free(w->hash);
  } 
  free(wt);
  return;
}


/* Part 3 - GESTION DES COMMITS */
/* FONCTION DE BASE */
kvp* createKeyVal(char* key,char* val) {//ok
  kvp* kv = (kvp*)malloc(sizeof(kvp));
  kv->key = strdup(key); 
  kv->value = strdup(val);
  return kv;
}
void freeKeyVal(kvp* kv) {//ok
  free(kv->key); 
  free(kv->value); 
  free(kv);
  return;
}
char* kvts(kvp* k) {//ok
  char* buff = (char*)malloc(MAX_INPUT);
  sprintf(buff,"%s : %s",k->key,k->value);
  return buff;
}
kvp* stkv(char* str) {//ok
  const char* delim = ":"; 
  char key[100], val[100];
  sscanf(str,"%s : %s",key,val);
  return createKeyVal(key,val);
}
Commit* initCommit() {//ok
  Commit* c = (Commit*)malloc(sizeof(Commit));
  c->n = 0; c->size = MAX_INPUT;
  c->T = (kvp**)malloc(c->size*sizeof(kvp*));
  for (int i=0;i<c->size;i++)
    c->T[i] = NULL;
  return c;
}
/* HASH FUNCTION */
int hash(char* str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) 
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}
void commitSet(Commit* c,char* key,char* value) {//ok
  int h = hash(key)%c->size;
  while (c->T[h]) 
    h = (h+1)%c->size;
  c->T[h] = createKeyVal(key,value);
  c->n++;
  return;
}
Commit* createCommit(char* hash) {//ok
  Commit* c = initCommit();
  commitSet(c,"tree",hash);
  //c->n++;
  return c;
}
char* commitGet(Commit* c,char* key) {//ok
  for(int i=0;i<c->size;i++) {
    if (c->T[i] && strcmp(c->T[i]->key,key)==0) {
      return c->T[i]->value;
    }
  }
  return NULL;
}
char* cts(Commit* c) {//ok
  char* desc = (char*)malloc(INT_MAX);
  char* w = NULL;
  for (int i=0;i<MAX_INPUT;i++) {
    if (c->T[i]) {
      w = kvts(c->T[i]);
      strcat(desc,w); strcat(desc,"\n");
    }
  }
  return desc;
}
Commit* stc(char* file) {//ok
  char str[(int)strlen(file)]; 
  sprintf(str,"%s",file);
  char* desc = str;
  char* ptr; 
  kvp* k; 
  Commit* c = initCommit();
  while((ptr = strtok_r(desc, "\n", &desc))) {
    k = stkv(ptr);
    commitSet(c,k->key,k->value);
  }
  return c;
}
void ctf(Commit* c,char* file) {//ok
  FILE* f = fopen(file,"w");
  if (!f) {
    printf("ctf: Erreur lors de l'ouverture\n");
    return;
  }
  fprintf(f,"%s",cts(c));
  fclose(f); 
  return;
}
Commit* ftc(char* file) {//ok
  Commit* c = initCommit();
  FILE* f = fopen(file,"r");
  if (!f) {
    printf("ftc: Erreur lors de l'ouverture de fichier\n");
    return NULL;
  }
  char buffer[256]; 
  char* res = fgets(buffer,256,f); 
  char* key; 
  char* value;
  while (res!=NULL) {
    key = strtok(buffer,":\n");
    value = strtok(NULL,":\n");
    commitSet(c,key,value);
    res = fgets(buffer,256,f);
  }
  fclose(f);
  return c;
}



/* Part 4 - GESTION D'UNE TIMELINE ARBORESCENTE */
/* FONCTION DE BASE */
void initBranch(void) {
  system("echo master > .current_branch");
  return;
}
int branchExists(char* branch) {
  return searchList(listdir(".refs"),branch)!=NULL;
}
void createBranch(char* branch) {
  createUpdateRef(branch,getRef("HEAD"));
  return;
}
char* getCurrentBranch(void) {
  FILE* f = fopen(".current_branch","r");
  char* buff = (char*)malloc(PATH_MAX);
  fgets(buff,PATH_MAX,f); 
  fclose(f); 
  return buff;
}
void printBranch(char* branch) {
  char* c_hash = getRef(branch);
  Commit* c = ftc(hashToPathCommit(c_hash));
  while (c) {
    if (commitGet(c,"message")) 
      printf("%s -> %s\n",c_hash,commitGet(c,"message"));
    else printf("%s\n",c_hash);

    if (commitGet(c,"predecessor")) {
      c_hash = commitGet(c,"predecessor");
      c = ftc(hashToPathCommit(c_hash));
    }
    else c = NULL;
  }
  free(c_hash);
  return;
}
List* branchList(char* branch) {
  char* c_hash = getRef(branch);
  Commit* c = ftc(hashToPathCommit(c_hash));
  List* L = initList();
  while (c) {
    insertFirst(L,buildCell(c_hash));
    if (commitGet(c,"predecessor")) {
      c_hash = commitGet(c,"predecessor");
      c = ftc(hashToPathCommit(c_hash));
    }
    else c = NULL;
  }
  free(c_hash);
  return L;
}
List* getAllCommits(void) {
  List* L = initList();
  List* ref = listdir(".refs");
  for (Cell* ptr=*ref;ptr!=NULL;ptr=ptr->next) {
    if (ptr->data[0]=='.') continue;
    List* branch = branchList(ptr->data);
    while (*branch) {
      if (!searchList(L,(*branch)->data)) 
        insertFirst(L,buildCell((*branch)->data));
      *branch = (*branch)->next;
    }
  }
  return L;
}