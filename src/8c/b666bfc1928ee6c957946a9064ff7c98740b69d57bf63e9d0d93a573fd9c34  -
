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

/* Side function */
int isFile(const char* fileName);
char* concat(char* s1,char* s2);
char* hashToFile(char* hash);
int isWorkTree(char* hash);

char* blobWorkTree(WorkTree* wt);
char* saveWorkTree(WorkTree* wt,char* path);
void restoreWorkTree(WorkTree* wt,char* path);


/* Part 3 - GESTION DES COMMITS */
/* Fonctions de base */
char* blobCommit(Commit* c);

/* MANIPULATION DES REFERENCES */
void initRefs();
void createUpdateRef(char* ref_name,char* hash);
void deleteRef(char* ref_name);
char* getRef(char* ref_name);

/* SIMULATION */
void myGitAdd(char* file_or_folder);
void myGitCommit(char* branch_name,char* message);



/* Part 4 - GESTION D'UNE TIMELINE ARBORESCENTE */
/* Side function */
char* hashToPathCommit(char* hash);

/* SIMULATION DE GIT CHECKOUT */
void restoreCommit(char* hash_commit);
void myGitCheckoutBranch(char* branch);
List* filterList(List* L,char* pattern);
void myGitCheckoutCommit(char* pattern);



/* Part 5+6 - FINAL */
/* SIMULATION DE GIT MERGE */
WorkTree* mergeWorkTree(WorkTree* wt1, WorkTree* wt2, List** conflicts);
List* merge(char* remote_branch, char* message);
void createDeletionCommit(char* branch, List* conflicts, char* message);

#endif