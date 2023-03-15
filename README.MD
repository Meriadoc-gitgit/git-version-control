# Projet: Réalisation d'un logiciel de gestion de versions
> **Aim:** Realisation d'un *outil de suivi et de versionnage de code* (**`git`**)
# Cadre du projet
Un logiciel de **gestion de versions** est un outil permettant le stockage, le suivi et la gestion de plusieurs versions d'un projet *(ou d'un ensembles de fichiers)*. 
- offrent un **acces aise a l'historique de toutes les modifications effectuees** sur les fichiers + permettent notamment de *recuperer une version anterieure en cas de probleme*. 
- tres utiles dans le cadre de travail collaboratif, permettant de *fusionner de maniere intelligente differentes versions d'un meme projet*. 
> **Objectif:** etudier le fonctionnement d'un logiciel de gestion de versions, *en detaillant* differentes structures de donnees impliquees dans sa mise en oeuvre. 
# Vers la creation d'enregistrements instantanes
>**Pt1:** Enregistrement d'un instantanee d'**un** fichier

Sous `git`, tous les objets, *qu'ils soient relatifs aux fichiers versionnes ou a leurs meta-donnees*, sont enregistres sous forme de fichiers. <br>
Ces fichiers ont pour particularite de pouvoir deriver le chemin ou ils sont stockes a partir de leur contenu. 
![fichiers-stocke.png](img/fichiers-stocke.png)

#### Note:
- Ici, la fonction de hachage `SHA256` est appliquee sur le contenu du fichier
- Le chemin ou doit etre stocke le fichier est obtenu en inserant `/` entre le 2e et 3e caracteres du hash. 

Faire dependre le chemin du contenu permet notamment de sauvegarder toutes les differentes versions du fichier. 


#### **Mofifier le contenu du fichier** va: 
- modifier le chemin vers lequel le sauvegarder 
- creer differentes sauvegardes correspondant a differentes etats du fichier. 

*(Quand on realise une telle **sauvegarde**, on dit communement qu'on "enregistre **un instantane**"/qu'on creer un "enregistrement instantane".)*
***
## Prise en main de Bash
Les donnees sur le disque sont stockees dans des fichiers, dont des structures de donnees qui apparaissent aux programmes comme des **suites finies d'octets**. 
> #### **File system**: 
>- La structure de donnees qui organise les fichiers sur le disque
>- Une des fonctionnalites principales d'un systeme d'exploitation

>Bash permet d'utiliser une interface en ligne de commande.
### Liste de commandes basiques en Bash
- `man`: afficher la documentation de la commande concernee
- `pwd`: afficher le repertoire courant
- `cd`: se positionner sur un certain repertoire de travail
- `mkdir`: creer un repertoire
- `touch` *`nom_de_fichier`*: creer le fichier de nom *`nom_de_fichier`*
- `ls`: lister les fichiers d'un repertoire
- `cat`: affichier le contenu d'un fichier
- `rm`: supprimer un fichier

#### Other:
- possible de rediriger une des sorties vers un fichier. <br>
  `ls > list.txt`: obtenir `list.txt` contenant la liste des fichiers et repertoires presents dans le repertoire courant *(plutot creer `list.txt`)*
- possible de rediriger la sortie d'une commande vers une autre, en utilisant **pipeline**. <br>
`cat noms.txt | sort`: lire la liste de noms presente dans le fichier `noms.txt` + la transmettre a la fonction `sort` qui va la trier *(ordre alphabetique)*

Linux met en place des flux globaux permettant a des commandes appelees, de dialoguer avec le programme appelant en ecrivant ou en lisant depuis ces flux.

#### Exemple de Bash de TP5
![Bash_command.png](img/Bash_command.png)

```shell
ls > file.tmp
cat file.tmp
cat main.c | sha256sum > file.tmp
cat file.tmp
```

### Les flux globaux:
- `stdin` *(entree standard)*: 
  - flux d'entree du programme
  - defaut: des donnees saisies au clavier
  - permet de definir des programmes interactifs, recuperant des donnees saisies sur le terminal par l'utilisateur *(par le biais de `scanf` etc.)*
- `stdout` *(sortie standard)*: 
  - la sortie du programme
  - defaut: du terminal ayant lance le programme
  - permet d'afficher des donnees sur le terminal
- `stderr` *(sortie standard d'arreur)*: 
  - sert a recuperer les messages d'erreurs
  - par defaut les afficher sur le terminal ayant lance le programme

### New stuff on C for me
- **Commande `sha256sum`:** hacher le contenu d'un fichier en utilisant la fonction de hachage `SHA256`.
- **Fonction `system`:** 
  - permet de faire des appels systeme. 
  - pas le seul moyen de manipuler le file system
  - 2 codes equivalentes: 
  ```C
  #include <stdio.h>
  #include <stdlib.h>
  int main(void) {
    FILE *f = fopen("test.txt","w");
    fprintf(f, "Test");
  }
  ```
  ```C
  #include <stdio.h>
  #include <stdlib.h>
  int main(void) {
    system("echo Test $>$ text.txt");
  }
  ```
    - `$>$`: write `Test` into `text.txt`
#### Librairie `<unistd>`
- permet une **gestion efficace des fichiers temporaires**
- utile pour stocker le hash d'un fichier en attente de lecture par le programme
- Fonction `mkstemp(fname)`: cree un fichier temporaire dont le nom stocke dans `fname`
  ```C
  static char template[] = "/tmp/myfileXXXXXX";
  char fname[1000];
  strcpy(fname, template);
  int fd = mktemps(fname);
  ```
  - faut que les 6 derniers caracteres de `template` soit `"XXXXXX"` pour que `mkstemps` les remplacent pour creer un nom de fichier unique.
  - `mkstemp` ouvre, *ensuite*, le fichier temporaire nouvellement cree + renvoie un descripteur de fichier ouvert *(lecture + ecriture)*

#### Librairie `<dirent.h>`
- Fonctions `opendir` et `readdir`: permettent d'explorer un repertoire
```C
DIR* dp = opendir(root_dir);
struct dirent *ep;
if (dp!=NULL) {
  while ((ep=readdir(dp))!=NULL) printf("\%s \n",ep->d_name);
}
```
# Source code pack
## Global usage
1. Fonction `int hashFile(char* source, char* dest)`: etant donne le chemin de 2 fichiers
    - calcule le hash du contenu du premier fichier
    - l'ecrit dans le deuxieme fichier

2. Fonction `char* sha256file(char* file)`: 
    - Requirement: 
      - la commande `sha256sum`
      - 1 pipe
      - 1 redirection
      - 1 fichier temporaire *(supprime apres usage!)*
    - renvoie 1 chaine de caracteres contenant *le hash du fichier donne en parametre*

## Implementation d'une **liste de chaines de caracteres**
```C
typedef struct cell {
  char* data;
  struct cell* next;
} Cell;
typedef Cell* List;
```
### Manipulating function
1. `List* initList()`: initialise une liste vide <br>
*(ne plus initialiser de liste autrement que par cette fonction)*
2. `Cell* buildCell(char* ch)`: allouer + retourner une cellule de la liste
3. `void insertFirst(List* L, Cell* C)`: ajouter un element en tete d'une liste
4. `char* ctos(Cell* c)`: retourne le chaine de caracteres qu'elle represente <br>
`char* ltos(List* L)`: transforme une liste en une chaine de caractere de format: `chain1|chaine2|...` *(utilisant fonction `ctos`)*
5. `Cell* listGet(List* Lm int i)`: renvoie le i-ieme element d'une liste
6. `Cell* searchList(List* L, char* str)`
    - recherche un element dans une liste a partir de son contenu
    - renvoie une reference vers lui / `NULL` si absent de la liste
7. `List stol(char* s)`: transformer une chaine de caracteres representant une liste en une liste chainee
8. `void ltof(List* L, char* path)`: ecrire une liste dans un fichier <br>
`List* ftol(chr* path)`: lire une liste enregistree dans un fichier
## Gestion de fichier sous `git`
> **Objectif:** produire une fonction qui enregistre un instantane *(realise une sauvegarde)* d'un fichier dont le nom est donne en parametre
1. `List* listdir(char* root_dir)`: 
    - prend en parametre une adresse
    - renvoie une liste contenant les noms des fichiers et repertoires qui s'y trouvent
2. `int file_exists(char* file)`: renvoie `1` si le fichier existe dans le repertoire courant et `0` sinon
3. `void cp(char* to, char* from)`: copie le contenu d'un fichier vers un autre *(en lisant ligne par ligne du fichier source)* <br>
**Note:** verifier que le fichier source existe avant !
4. `char* hashToPath(char* hash)`: retourne le chemin d'un fichier a partir de son hash
5. `void blobFile(char* file)`: enregistre un instantane *(realise un sauvegarde)* du fichier donnee en entree <br>
**Note:** en utilisant `mkdir`

# Enregistrement de plusieurs instantanes
>**Pt2:** Enregistrement d'un instantanee de **plusieurs** fichiers et/ou repertoires du projet.

>## Objectif 
>- pouvoir creer un enregistrement instantane d'un `WorkTree` et de son contenu
>- *(puis)* permettre la restauration de cet ensemble de fichier
>
>*(comme decrit dans ces enregistrements instantanes)*

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