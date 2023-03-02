#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<assert.h>
#include"cell.h"

List* listdir(char* root_dir){
    DIR * dp = opendir (root_dir);
    struct dirent * ep ;
    List* liste=initList();
    Cell*cellule=NULL;
    if( dp != NULL ){
        while (( ep = readdir ( dp ) ) != NULL){
            cellule=buildCell(ep->d_name);
            insertFirst(liste,cellule);
        }    
    }
    return liste;
}

 int file_exists(char *file){
    char *D_current;
    printf("saisissez un nom de repertoire\n");
    scanf("%s",&D_current);
    List*liste=listdir(D_current);
    Cell*L_cpy=(*liste);
    while(L_cpy!=NULL){
        if((L_cpy->data)==file) return 1;
        L_cpy=L_cpy->next;
    }
    return 0;
}

void cp(char *to, char *from){
    assert(from!=NULL);
    FILE*src=fopen(from,"r");
    FILE*dst=fopen(from,"w");
    char buffer[300];
    while(fgets(buffer,300,src)){
        strcat(buffer,"\n");
        fputs(buffer,dst);
    }
    fclose(src);
    fclose(dst);   
}

char* hashToPath(char* hash){
    int i=3;
    char*chemin=NULL;
    chemin[0]=hash[0];
    chemin[1]=hash[1];
    chemin[2]=hash[2];
    chemin[3]='/';
    while(hash!='\0'){
        chemin[i]=hash[i];
        i+=1;
    }
    return chemin;

}
void blobFile(char* file){
    
}