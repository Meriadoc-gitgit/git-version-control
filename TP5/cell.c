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
  if (*L==NULL) {
    *L = C; 
    printf("ok\n");
  }
  else {
    C->next = *L;
    *L = C;
    printf("ko\n");
  }
  return;
}
char* ctos(Cell* C) {
  return C->data;
}
char* ltos(List* L) {
  if ((*L)==NULL) {
    printf("Erreur de pointeur vers List* L de Cell\n");
    return NULL;
  }
  char* rst = "";
  char tmp[256], tmp2[256];
  char* delim = "|";

  while((*L)) {
    strcpy(tmp,strdup(ctos(*L)));
    strcat(tmp,delim);
    printf("\n%s\n",tmp);

    strcpy(tmp2,strdup(tmp));
    printf("\ntmp=%s\n",tmp);
    printf("\ntmp2=%s\n",tmp2);
    //strcat(rst,tmp2);
    //strcat(rst,"|");
    (*L) = (*L)->next;
  }
  return rst;
}
Cell* listGet(List* L,int i) {
  Cell* tmp = *L;
  while(i>1) {
    tmp = tmp->next;
    i--;
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
  FILE *f = fopen(path,"w");
  Cell* tmp = *L;
  
  while(tmp) {
    fprintf(f,"%s\n",tmp->data);
    tmp = tmp->next;
  }
  fclose(f);
  return;
}
List* ftol(char* path) {
  FILE *f = fopen(path,"r");
  List* L = initList();

  char buffer[256];
  char *res = fgets(buffer, 256, f);
  char data[26];
  
  while(res) {
    sscanf(buffer,"%s",data);
    insertFirst(L,buildCell(data));
    char *res = fgets(buffer, 256, f);
  }
  fclose(f);

  return L;
}