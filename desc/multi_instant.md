# ENREGISTREMENTS DE PLUSIEURS INSTANTANES
**Pt2:** Enregistrement d'un instantanée de **plusieurs** fichiers et/ou répertoires du projet.

## Objectif 
- pouvoir créer un enregistrement instantané d'un `WorkTree` et de son contenu
- *(puis)* permettre la restauration de cet ensemble de fichier

*(comme decrit dans ces enregistrements instantanés)*

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
Un `WorkFile` représente un fichier ou répertoire dont on souhaite enregistrer un instantané.
Il possède 3 champs: 
- `name`: nom du fichier ou du répertoire
- `hash`: hash associé à son contenu, initialisé à `NULL`
- `mode`: les autorisations associés au fichier *(modification, lecture et exécution)*, initialisé à `0`.

Un `WorkTree` = un tableau de `WorkFile`.
### Note: Les autorisations
- associés à un fichier, décrivent qui peut le modifier, le lire et l'exécuter
- sont représentées par un nombre de 3 digits en octal

  >**Dangereux:** `777` = donne à tout le monde le droit de lire, écrire et exécuter le fichier
#### Fonctions fournie pour manipuler les `Chmod`
- `getChmod`: récupérer ces autorisations
- `setChmod`: modifier les autorisations

## Fonctions de manipulation de base
### Manipulation de `WorkFile`
- `WorkFile* createWorkFile(char* name)`: créer un `WorkFile` + l'initialiser
- `char* wfts(WorkFile* wf)`: convertir un `WorkFile` en chaine de caractàres contenant les différents champs séparés par des tabulations *(caractères `'\t'`)*
- `WorkFile* stwf(char* ch)`: convertir une chaine de caractères représentant un `WorkFile` en un `WorkFile`
### Manipulation de `WorkTree`
- `WorkTree* initWorkTree()`: allouer un `WorkTree` de taille fixée *(donnée par une constante du programme)* + l'initialiser
- `int inWorkTree(WorkTree* wt,char* name)`: vérifie la présence d'un fichier ou répertoire dans un `WorkTree`, retourner:
	- la position du fichier dans le tableau, si présent
	- `-1` sinon
- `int appendWorkTree(WorkTree* wt,char* name,char* hash,int mode)`: ajoute un fichier ou répertoire au `WorkTree` *(s'il n'existe pas déjà)*
- `char* wtts(WorkTree* wt)`: convertit un `WorkTree` en une chaine de caractères composée des représentations des `WorkFile` separées par un saut de ligne *(caractère `'\n'`)*
- `WorkTree* stwt(char* ch)`: convertit une chaine de caractères représentant un `WorkTree` en un `WorkTree`
- `int wttf(WorkTree* wt,char* file)`: écrit dans le fichier `file` la chaine de caractères représentant un `WorkTree`
- `WorkTree* ftwt(char* file)`: construit un `WorkTree` à partir d'un fichier qui contient sa représentation en chaine de caractères

## Enregistrement et restauration
### Preliminary
Réaliser un enregistrement instantané d'un `WorkTree` = créer un enregistrement instantané du fichier qui le représente. 

Pour pouvoir le distinguer d'un fichier classique, on ajoute l'extension `.t` au nom de son enregistrement instantané. 

Example: 
- hash du fichier représentant le WorkTree: `dsfsd245azd`
- l'enregistrement instantané est dans: `ds/fsd245azd.t`
#### Fonction
```c
char* blocWorkTree(WorkTree* wt);
```
#### Algorithm
- crée un fichier temporaire représentant le `WorkTree` 
- *ensuite,* créer l'enregistrement instantané du `WorkTree` *avec l'extension `.t`*

**Retour:** hash du fichier temporaire
***
### Sauvegarder un `WorkTree`
#### Fonction
```c
char* saveWorkTree(WorkTree* wt,char* path);
```
#### Mission
Étant donné un `WorkFile` *(fichier ou répertoire!)* dont le chemin est donné en paramètre: 
- crée un enregistrement instantané de tout son contenu *(de manière récursive)*
- *puis* de lui même

#### Algorithme
La fonction parcourt le tableau de `WorkFile` de ` WorkTree wt`, et pour chaque `WorkFile WF`: 
- Si `WF` = fichier: 
	- `blobFile` pour créer un enregistrement instantané de ce fichier
	- récupère le hash du fichier et son mode `getChmod` pour le sauvegarder dans `WF`
- Sinon, réaliser un appel récursif sur ce répertoire
	- créer un nouveau `WorkTree newWT` *(représentant tout le contenu de ce répertoire: `listdir`)*
	- réaliser un appel récursif sur `newWT`
	- *puis,* récupérer son hash et son mode pour le sauvegarder dans `WF`

*** 
### Restaurer un `WorkTree`
#### Fonction
```c
void restoreWorkTree(WorkTree* wt, char* path);
```
#### Mission
Recréer l'arborescence des fichiers comme décrit par ses enregistrements instantanés. 
#### Algorithm
La fonction parcourt le tableau de `wt`, en réalisant pour chacun de ses `WorkFile WF` le traitement suivant: 
- trouver l'enregistrement instantané correspondant au hash de `WF`
- Si l'enregistrement ne possède pas de `.t`: **fichier**
	- créer une copie de l'enregistrement à l'endroit indiqué par `path`
	- lui donner le nom et les autorisations correspondant aux champs `name` et `mode`
- Sinon, 
	- créer le `WorkTree` associé
	- modifier `path`: ajouter ce répertoire à la fin
	- faire un appel récursif

> **Note:** pensez à sauvegarder votre code dans un autre répertoire avant de tester cette fonction, au risque de supprimer tout le projet !

