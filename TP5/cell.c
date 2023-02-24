#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cell.h"

List* initList() {
  List* rst = (List*)malloc(sizeof(List));
  return rst;
}
Cell* buildCell(char* ch) {
  Cell* rst = (Cell*)malloc(sizeof(Cell));
  rst->data = strdup(ch);
  rst->next = NULL;
  return rst;
}
void insertFirst(List* L,Cell* C) {
  Cell* tmp = &L;
  C->next = tmp;
  L = tmp;
  return;
}
char* ctos(Cell* C) {
  return C->data;
}
char* ltos(List* L) {
  Cell* tmp = &L;
  char* rst = "";
  if (!tmp) {
    printf("Erreur de pointeur vers List* L de Cell");
    return;
  }
  while(tmp) {
    strcat(rst,tmp->data);
    strcat(rst,"|");
    tmp = tmp->next;
  }
  return rst;
}
Cell* listGet(List* L,int i) {
  Cell* tmp = &L;
  while(i>0) tmp = tmp->next;
  if (i==0) return tmp;
  else return NULL;
}
Cell* searchList(List* L,char* str) {
  Cell* tmp = &L;
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
  char* delim = "|";

  char* ptr = strtok(str,delim);
  List* L = initList();

  while (ptr!=NULL) {
    insertFirst(L,buildCell(ptr));
    ptr = strtok(NULL,delim);
  }
  return L;
}
void ltof(List* L,char* path) {
  
}