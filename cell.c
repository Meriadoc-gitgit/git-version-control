#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "cell.h"


List* initList(){
  List* Liste=(List*)malloc(sizeof(List));
  (*Liste)->next=NULL;
  return Liste;
}

Cell* buildCell(char* ch){
  Cell*cellule=(Cell*)malloc(sizeof(Cell));
  cellule->data=strdup(ch);
  cellule->next=NULL;
  return cellule;
}

void insertFirst(List *L, Cell *C){
  if(L==NULL){
    *L=C;
  }
  else{
    C->next=(*L);
    *L=C;
  }
  return;
}

char* ctos(Cell* c){
  return puts(c->data);
}
char* ltos(List* L) {
  char *output="";
  while ((*L)) {
    strcat(output, ctos(*L));
    strcat(output,"|");
    *L = *L->next;
  }
  return output;
}
Cell* listGet(List *L, int i) {
  while (i>0) {
    *L = (*L)->next;
    i--;
  }
  return L;
}
Cell* searchList(List* L, char* str) {
  while (L) {
    if (strcmp(str, (*L)->data)==0) 
    return L;
    L = L->next;
  }
  return NULL;
}
int main(void) {
  List *L = initList();
  Cell*c=buildCell("maCell");
  insertFirst(L,c);

  return 0;
}
