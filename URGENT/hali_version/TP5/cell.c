#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "cell.h"


List* initList(){
  List*Liste=(List*)malloc(sizeof(List));
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
    (*L)=C;
  }
  return;
}

char* ctos(Cell* c){
  puts(c->data);
   return c->data;
}
char* ltos(List* L) {
  char *output="";
  while (L) {
    strcat(output, ctos(*L));
    strcat(output,"|");
    *L = (*L)->next;
  }
  return output;
}

Cell* listGet(List *L, int i) {
  while (i>0) {
    *L = (*L)->next;
    i--;
  }
  return *L;
}

Cell* searchList(List* L, char* str) {
  while (L) {
    if (strcmp(str, (*L)->data)==0) return *L;
    *L =(*L)->next;
  }
  return NULL;
}
List* stol(char* s){
  return NULL;
}
// Ecriture de fichier
void ltof(List* L, char* path){
  FILE*f=fopen(path,"w");
  if(f==NULL){
    printf("Erreur d'ouverture du fichier\n");
    return;
  }
  Cell* c=*L;
  while((c)){
    fprintf(f,"data:\n",c->data);
    c=c->next;
  }
  printf("fin");
  fclose(f);
}
//Lecture de fichier
List* ftol(char* path){
  FILE*f=fopen(path,"r");
  if(f==NULL){
    printf("Erreur d'ouverture du fichier\n");
    return NULL;
  }
  List* liste_lu=initList();
  char buffer[100];
  char* data;
  while(strncmp(buffer,"fin",3)!=0){
    sscanf(buffer,"data:%s\n",&data);
    Cell*cell_lu=buildCell(data);
    insertFirst(liste_lu,cell_lu);
    fgets(buffer,100,f);
  }
  fclose(f);
  return liste_lu;
}
