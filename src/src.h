#ifndef _SRC
#define _SRC

/* CELL - LIST */
typedef struct cell { 
  char* data;
  struct cell* next;
} Cell;

typedef Cell* List;

/* WORK[FILE|TREE] */
typedef struct {
  char* name;
  char* hash;
  int mode;
} WorkFile;

typedef struct {
  WorkFile* tab;
  int size;
  int n;
} WorkTree;


/* COMMIT */
typedef struct key_value_pair {
  char* key;
  char* value;
} kvp;

typedef struct hash_table {
  kvp** T;
  int n;
  int size;
} HashTable;

typedef HashTable Commit;


/* Part 1 - MANIPULATION DE CELL ET LIST */
List* initList();
Cell* buildCell(char* ch);
void insertFirst(List* L,Cell* C);
char* ctos(Cell* C);
char* ltos(List* L);
Cell* listGet(List* L,int i);
Cell* searchList(List* L,char* str);
List* stol(char* s);
void ltof(List* L,char* path);
List* ftol(char* path);


/* Part 2 */
/* MANIPULATION DE WORKFILE */
WorkFile* createWorkFile(char* name);
char* wfts(WorkFile* wf);
WorkFile* stwf(char* ch);
void libererWorkFile(WorkFile* wf);

/* MANIPULATION DE WORKTREE */
WorkTree* initWorkTree();
int inWorkTree(WorkTree* wt,char* name);
int appendWorkTree(WorkTree* wt,char* name,char* hash,int mode);
char* wtts(WorkTree* wt);
WorkTree* stwt(char* ch);
int wttf(WorkTree* wt,char* file);
WorkTree* ftwt(char* file);
void libererWorkTree(WorkTree* wt);


/* Part 3 - GESTION DES COMMITS */
/* FONCTION DE BASE */
kvp* createKeyVal(char* key,char* val);
void freeKeyVal(kvp* kv);
char* kvts(kvp* k);
kvp* stkv(char* str);
Commit* initCommit();
int hash(char *str); //choix de hash !
void commitSet(Commit* c,char* key,char* value);
Commit* createCommit(char* hash);
char* commitGet(Commit* c,char* key);
char* cts(Commit* c);
Commit* stc(char* file);
void ctf(Commit* c,char* file);
Commit* ftc(char* file);

#endif