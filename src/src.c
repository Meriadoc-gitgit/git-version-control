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
    ptr = strtok(NULL,delim); printf("ptr: %s\n",ptr);
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
    if (strcmp(wt->tab->name,name)==0) return i;
  }
  return -1;
}
int appendWorkTree(WorkTree* wt,char* name,char* hash,int mode) {
  if (inWorkTree(wt,name)==-1 && wt->n<wt->size) {
    WorkFile* wf = createWorkFile(name);
    wf->hash = strdup(hash); wf->mode = mode;
    wt->tab[wt->n] = *wf; wt->n++;
    return 1;
  }
  return 0;
}
char* wtts(WorkTree* wt) {
  char command[MAX_INPUT]; char* w;
  for (int i=0;i<wt->n;i++) {
    w = wfts(&(wt->tab[i]));
    sprintf(command,"%s%s\n",command,w);
  }
  char* res = (char*)malloc(sizeof(char));
  strcpy(res,command);
  return res;
}
WorkTree* stwt(char* ch) {
  char str[(int)strlen(ch)]; sprintf(str,"%s",ch);
  const char* delim = "\n";
  char* ptr = strtok(str,delim);
  WorkTree* wt = initWorkTree();

  while(ptr!=NULL) {
    WorkFile* wf = stwf(ptr);
    appendWorkTree(wt,wf->name,wf->hash,wf->mode);
    ptr = strtok(NULL,delim);
  }
  return wt;
}