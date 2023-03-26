#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "bash.h"
#include "src.h"

/* Part 1 - Manipulation de Cell et List */
List* initList() {
  List* rst = (List*)malloc(sizeof(Cell));
  return rst;
}
Cell* buildCell(char* ch) {
  Cell* rst = (Cell*)malloc(sizeof(Cell));
  rst->data = strdup(ch);
  rst->next = NULL;
  return rst;
}
void insertFirst(List* L,Cell* C) {
  C->next = *L; *L = C;
  return;
}
char* ctos(Cell* C) {
  if (C==NULL) return NULL;
  return C->data;
}
char* ltos(List* L) {
  if (*L==NULL) {
    printf("Liste *L est NULL\n");
    return NULL;
  }
  char command[MAX_INPUT] = "";
  Cell* C = *L;
  char* rst = (char*)malloc(sizeof(char)), *tmp;
  while(C) {
    tmp = ctos(C);
    sprintf(command,"%s%s|",command,tmp);
    C = C->next;
  }
  strcpy(rst,command);
  return rst;
}
Cell* listGet(List* L,int i) {
  if (*L==NULL) return NULL;
  Cell* tmp = *L;
  if (i==0) return NULL;
  while(i>1) {
    tmp = tmp->next; i--;
  }
  if (i==1) return tmp;
  else return NULL;
}
Cell* searchList(List* L,char* str) {
  Cell* tmp = *L;
  while(tmp) {
    if (strcmp(tmp->data,str)==0) return tmp;
    tmp = tmp->next;
  }
  return NULL;
}
List* stol(char* s) {
  char str[(int)strlen(s)]; sprintf(str,"%s",s);
  const char* delim = "|";
  char* ptr = strtok(str,delim); //printf("ptr0: %s\n",ptr);
  List* L = initList();

  while (ptr!=NULL) {
    insertFirst(L,buildCell(ptr)); //printf("ptr: %s\n",ptr);
    ptr = strtok(NULL,delim); //printf("ptr: %s\n",ptr);
  }
  //printf("stol:ok\n");
  return L;
}
void ltof(List* L,char* path) {
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
    fprintf(f,"%s\n",C->data); //printf("ok\n");
    C = C->next;
  }
  fclose(f);
  return;
}
List* ftol(char* path) {
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
  char buffer[256]; char *res = fgets(buffer, 256, f); char data[26];
  while(res!=NULL) {
    sscanf(buffer,"%s",data);
    res = fgets(buffer, 256, f);
    insertFirst(L,buildCell(data));
  }
  fclose(f);
  return L;
}
void libererWorkFile(WorkFile* wf) {
  free(wf->name); free(wf->hash); free(wf);
  return;
}



/* Part 2 */
/* MANIPULATION DE WORKFILE */
WorkFile* createWorkFile(char* name) {
  WorkFile* wf = (WorkFile*)malloc(sizeof(WorkFile));
  wf->name = strdup(name);
  wf->hash = NULL; wf->mode = 0;
  return wf;
}
char* wfts(WorkFile* wf) {
  if (!wf) {
    printf("Erreur wfts: WF NULL");
    return NULL;
  }
  char command[100];
  sprintf(command,"%s\t%s\t%d",wf->name,wf->hash,wf->mode);
  char* res = (char*)malloc(sizeof(char));
  strcpy(res,command);
  return res;
}
WorkFile* stwf(char* ch) {
  char str[(int)strlen(ch)]; sprintf(str,"%s",ch);
  const char* delim = "\t";
  char* ptr = strtok(str,delim);
  WorkFile* wf = createWorkFile(ptr);
  ptr = strtok(NULL,delim); wf->hash = strdup(ptr);
  ptr = strtok(NULL,delim); wf->mode = atoi(ptr);
  return wf;
}


WorkTree* initWorkTree() {
  WorkTree* wt = (WorkTree*)malloc(sizeof(WorkTree));
  wt->size = MAX_INPUT; wt->n = 0;
  wt->tab = (WorkFile*)malloc(wt->size*sizeof(WorkFile));
  return wt;
}
int inWorkTree(WorkTree* wt,char* name) {
  if (!wt) {
    printf("inWorkTree: wt NULL\n"); return -1;
  }
  for (int i=0;i<wt->n;i++) {
    if (strcmp(wt->tab->name,name)==0) {
      printf("%s\t%s\n",wt->tab->name,name); return i;
    }
  }
  return -1;
}
int appendWorkTree(WorkTree* wt,char* name,char* hash,int mode) {
  if (inWorkTree(wt,name)==-1 && wt->n<wt->size) {
    wt->tab[wt->n].name = strdup(name);
    wt->tab[wt->n].hash = strdup(hash);
    wt->tab[wt->n].mode = mode;
    wt->n++;
    return 0;
  }
  return -1;
}
char* wtts(WorkTree* wt) {
  if (wt->n==0) return "wtts: WorkTree vide\n";
  char *command = (char*)malloc(INT_MAX);
  char* w = NULL;
  for (int i=0;i<wt->n;i++) {
    w = wfts(&(wt->tab[i]));
    strcat(command,w); strcat(command,"\n");
  }
  return command;
}
WorkTree* stwt(char* ch) {
  char str[(int)strlen(ch)]; sprintf(str,"%s",ch);
  const char* delim = "\n";
  char* ptr = strtok(str,delim); 
  WorkTree* wt = initWorkTree(); WorkFile* wf;

  while(ptr!=NULL) {
    wf = stwf(ptr);
    appendWorkTree(wt,wf->name,wf->hash,wf->mode);
    ptr = strtok(NULL,delim);
  }
  return wt;
}
int wttf(WorkTree* wt,char* file) {
  FILE* f = fopen(file,"w");
  if (!f) {
    printf("wttf: Erreur lors de l'ouverture de fichier\n"); return -1;
  }
  fprintf(f,"%s",wtts(wt));
  fclose(f); return 0;
}
WorkTree* ftwt(char* file) {
  FILE* f = fopen(file,"r");
  if (!f) {
    printf("ftwt: Erreur lors de l'ouverture\n");
    return NULL;
  }
  WorkTree* wt = initWorkTree();
  char buffer[256]; char* res = fgets(buffer,256,f); char name[26], hash[256]; int mode;
  while (res!=NULL) {
    sscanf(buffer,"%s\t%s\t%d",name,hash,&mode);
    appendWorkTree(wt,name,hash,mode);
    res = fgets(buffer,256,f);
  }
  fclose(f);
  return wt;
}
void libererWorkTree(WorkTree* wt) {
  if (wt->n==0) return;
  for(int i=0;i<wt->n;i++) 
    libererWorkFile(&wt->tab[i]);

  free(wt);
  return;
}


/* Part 3 - GESTION DES COMMITS */
/* FONCTION DE BASE */
kvp* createKeyVal(char* key,char* val) {
  kvp* kv = (kvp*)malloc(sizeof(kvp));
  kv->key = strdup(key); kv->value = strdup(val);
  return kv;
}
void freeKeyVal(kvp* kv) {
  free(kv->key); free(kv->value); free(kv);
  return;
}
char* kvts(kvp* k) {
  char* s = (char*)malloc(MAX_INPUT);
  strcat(s,k->key); strcat(s,":"); strcat(s,k->value);
  return s;
}
kvp* stkv(char* str) {
  char ch[(int)strlen(str)]; sprintf(ch,"%s",str);
  const char* delim = ":"; char* key, *val;
  char* ptr = strtok(ch,delim); key = strdup(ptr);
  ptr = strtok(NULL,delim); val = strdup(ptr);
  return createKeyVal(key,val);
}
Commit* initCommit() {
  //A revoir
  Commit* c = (Commit*)malloc(sizeof(Commit));
  c->n = 0; c->size = MAX_INPUT;
  c->T = (kvp**)malloc(c->size*sizeof(kvp));
  return c;
}
/* HASH FUNCTION */
unsigned long hash(char* str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) 
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash % MAX_INPUT;
}
void commitSet(Commit* c,char* key,char* value) {
  unsigned long h = hash(key);
  int insertOK = 0;
  if (c->T[h]) {
    for (int i=h;i<MAX_INPUT;i++) {
      if (!c->T[i]) {
        c->T[i] = createKeyVal(key,value);
        c->n++; insertOK = 1; break;
      }
    }
  } else {
    c->T[h] = createKeyVal(key,value);
    c->n++; insertOK = 1;
  }
  if (!insertOK) {
    for (int i=0;i<h;i++) {
      if (!c->T[i]) {
        c->T[i] = createKeyVal(key,value);
        c->n++; insertOK = 1; break;
      }
    }
  }
  if (!insertOK) printf("commitSet: Surcharge de memoire\n");
  return;
}
Commit* createCommit(char* hash) {
  Commit* c = initCommit();
  commitSet(c,"tree",hash);
  c->n++;
  return c;
}
char* commitGet(Commit* c,char* key) {
  for(int i=0;i<MAX_INPUT;i++) {
    if (c->T[i] && strcmp(c->T[i]->key,key)==0) {
      return c->T[i]->value;
    }
  }
  return NULL;
}
char* cts(Commit* c) {
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
void ctf(Commit* c,char* file) {
  char str[(int)strlen(file)]; sprintf(str,"%s",file);
  char* desc = str;
  char* ptr; kvp* k;
  while((ptr = strtok_r(desc, "\n", &desc))) {
    k = stkv(ptr);
    commitSet(c,k->key,k->value);
  }
  return;
}