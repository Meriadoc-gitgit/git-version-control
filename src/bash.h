#ifndef _BASH
#define _BASH

#include "src.h"

/* Part 1 - Enregistrement d'un instantanee d'un fichier */
int hashFile(char* src, char *dst);
char* sha256file(char* file);

List* listdir(char* root_dir);
int file_exists(char* file);
void cp(char* to, char* from);
char* hashToPath(char* hash);
void blobFile(char* file);


/* Part 2 - Enregistrement d'un instantanee de plusieurs */
int getChmod(const char* path);
void setMode(int mode,char* path);

char* blobWorkTree(WorkTree* wt);
char* concat(char* s1,char* s2);
char* saveWorkTree(WorkTree* wt,char* path);
void restoreWorkTree(WorkTree* wt,char* path);

#endif