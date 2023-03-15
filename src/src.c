#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  char delim = '|'; Cell* C = *L;
  char* rst = (char*)malloc(sizeof(char)), *tmp; int temp = 0;
  while(C) {
    tmp = ctos(C);
    for (int i=0;i<(int)strlen(tmp);i++) 
      rst[i+temp] = tmp[i];
    temp += (int)strlen(tmp)+1; 
    rst[temp-1] = delim;
    C = C->next;
  }
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
  char str[(int)strlen(s)];
  for(int i=0;i<(int)strlen(s);i++)
    str[i] = s[i];
  int init_size = (int)strlen(str);
  const char* delim = "|";
  char* ptr = strtok(str,delim);
  List* L = initList();

  while (ptr!=NULL) {
    insertFirst(L,buildCell(ptr)); //printf("ptr: %s\n",ptr);
    ptr = strtok(NULL,delim);
  }
  //printf("stol:ok\n");
  return L;
}
void ltof(List* L,char* path) {
  if (*L==NULL) {
    printf("Liste *L est NULL\n");
    return;
  }
  FILE *f = fopen(path,"w");
  if (!f) {
    printf("Erreur lors de l'ouverture\n");
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
    printf("Fichier introuvable\n");
    return NULL;
  }
  FILE *f = fopen(path,"r");
  if (!f) {
    printf("Erreur lors de l'ouverture\n");
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
  WorkFile* wt = (WorkFile*)malloc(sizeof(WorkFile));
  wt->name = strdup(name);
  wt->hash = NULL; wt->mode = 0;
  return wt;
}