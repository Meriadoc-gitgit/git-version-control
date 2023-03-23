# ENREGISTREMENTS DE PLUSIEURS INSTANTANES
**Pt2:** Enregistrement d'un instantanee de **plusieurs** fichiers et/ou repertoires du projet.

## Objectif 
- pouvoir creer un enregistrement instantane d'un `WorkTree` et de son contenu
- *(puis)* permettre la restauration de cet ensemble de fichier

*(comme decrit dans ces enregistrements instantanes)*

## Prelimiraire
```c
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
```
Un `WorkFile` represente un fichier ou repertoire dont on souhaite enregistre un instantane.<br>
Il possede 3 champs: 
- `name`: nom du fichier ou du repertoire
- `hash`: hash associe a son contenu, initialise a `NULL`
- `mode`: les autorisations associes au fichier *(modification, lecture et execution)*, initialise a `0`.

Un `WorkTree` = un tableau de `WorkFile`.
### Note: Les autorisations
- associes a un fichier, decrivent qui peut le modifier, le lire et l'executer
- sont representees par un nombre de 3 digits en octal

  >**Dangereux:** `777` = donne a tout le monde le droit de lire, ecrire et executer le fichier
#### Fonctions fournie pour manipuler les `Chmod`
- `getChmod`: recuperer ces autorisations
- `setChmod`: modifier les autorisations

## Fonctions de manipulation de base
### Manipulation de `WorkFile`
- `WorkFile* createWorkFile(char* name)`: creer un `WorkFile` + l'initialiser
- `char* wfts(WorkFile* wf)`: convertir un `WorkFile` en chaine de caracteres contenant les differents champs separes par des tabulations *(caracteres `'\t'`)*
- `WorkFile* stwf(char* ch)`: convertir une chaine de caracteres representant un `WorkFile` en un `WorkFile`
### Manipulation de `WorkTree`
- `WorkTree* initWorkTree()`: allouer un `WorkTree` de taille fixee *(donnee par une constante du programme)* + l'initialiser
- `int inWorkTree(WorkTree* wt,char* name)`: verifie la presence d'un fichier ou repertoire dans un `WorkTree`, retourner:
  - la position du fichier dans le tableau, si present
  - `-1` sinon
- `int appendWorkTree(WorkTree* wt,char* name,char* hash,int mode)`: ajoute un fichier ou repertoire au `WorkTree` *(s'il n'existe pas deja)*
- `char* wtts(WorkTree* wt)`: convertit un `WorkTree` en une chaine de caracteres composee des representations des `WorkFile` separees par un saut de ligne *(caractere `'\n'`)*
- `WorkTree* stwt(char* ch)`: convertit une chaine de caracteres representant un `WorkTree` en un `WorkTree`
- `int wttf(WorkTree* wt,char* file)`: ecrit dans le fichier `file` la chaine de caracteres representant un `WorkTree`
- `WorkTree* ftwt(char* file)`: construit un `WorkTree` a partir d'un fichier qui contient sa representation en chaine de caracteres

## Enregistrement et restauration
### Preliminary
Realiser un enregistrement instantate d'un WorkTree = creer un enregistrement instantane du fichier qui le represente. 

Pour pouvoir le distinguer d'un fichier classique, on ajoute l'extension `.t` au nom de son enregistrement instantane. 

Example: 
- hash du fichier representant le WorkTree: `dsfsd245azd`
- l'enregistrement instantane est dans: `ds/fsd245azd.t`
#### Fonction
```c
char* blocWorkTree(WorkTree* wt);
```
#### Algorithm
- cree un fichier temporaire representant le `WorkTree` 
- *ensuite,* creer l'enregistrement instantane du `WorkTree` *avec l'extension `.t`*

**Retour:** hash du fichier temporaire
***
### Sauvegarder un `WorkTree`
#### Fonction
```c
char* saveWorkTree(WorkTree* wt,char* path);
```
#### Mission
Etant donne un `WorkFile` *(fichier ou repertoire!)* dont le chemin est donne en parametre: 
- cree un enregistrement instantane de tout son contenu *(de maniere recursive)*
- *puis* de lui meme

#### Algorithme
La fonction parcourt le tableau de `WorkFile` de ` WorkTree wt`, et pour chaque `WorkFile WF`: 
- Si `WF` = fichier: 
  - `blobFile` pour creer un enregistrement instantane de ce fichier
  - recupere le hash du fichier et son mode `getChmod` pour le sauvegarder dans `WF`
- Sinon, realiser un appel recursif sur ce repertoire
  - creer un nouveau `WorkTree newWT` *(representant tout le contenu de ce repertoire: `listdir`)*
  - realiser un appel recursif sur `newWT`
  - *puis,* recuperer son hash et son mode pour le sauvegarder dans `WF`

*** 
### Restaurer un `WorkTree`
#### Fonction
```c
void restoreWorkTree(WorkTree* wt, char* path);
```
#### Mission
Recreer l'arborescence des fichiers comme decrit par ses enregistrements instantanes. 
#### Algorithm
La fonction parcourt le tableau de `wt`, en realisant pour chacun de ses `WorkFile WF` le traitement suivant: 
- trouver l'enregistrement instantane correspondant au hash de `WF`
- Si l'enregistrement ne possede pas de `.t`: fichier
  - creer une copie de l'enregistrement a l'endroit indique par `path`
  - lui donner le nom et les autorisations correspondant aux champs `name` et `mode`
- Sinon, 
  - creer le `WorkTree` associe
  - modifier `path`: ajouter ce repertoire a la fin
  - faire un appel recursif

> **Note:** pensez a sauvegarder votre code dans un autre repertoire avant de tester cette fonction, au risque de supprimer tout le projet !

