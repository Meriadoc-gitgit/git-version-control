# Gestion d'une timeline arborescente 
## Préliminaire
**Grand avantage de travailler avec plusieurs branches**: 
- pouvoir considérer plusieurs *versions* d'un *même code*
- permet à plusieurs personnes de: 
	- travailler simultanément sur différentes parties/fonctionnalités d'un projet
	- considérer des versions alternatives d'un code
	- *ou encore,* réaliser des modifications sans affecter la branche principale

![Timeline](sub_img/timeline.png)
- `master`: la branche principale
- `Feature1`: l'implémentation de la première fonctionnalité d'un projet

Dans la répertoire `.refs`, il y aura donc: 
- une référence appelée `master` contenant le hash du **dernier commit** de la branche **principale**
- une référence appelée `Feature1` contenant le hash du **dernier commit** de cette branche **parallèle**

#### Note
- Plusieurs branches peuvent exister simultanément
- Fichier caché `.current_branch`: 
	- permet à tout moment de savoir dans quelle branche nous nous situons
	- contient le nom de la branche courante

Avec plusieurs branches, il est primordial de pouvoir: 
- manipuler facilement une branche
- savoir comment **naviguer entre les différentes branches** de l'arborescence

## Fonction de base
1. `void initBranch()`: crée le fichier caché `.current_branch` contenant la chaine de caractères `master`
2. `int branchExists(char* branch)`: vérifie l'existence d'une branche
3. `void createBranch(char* branch)`: crée une référence appelée `branch` *(qui pointe vers le même commit que la référence `HEAD`)* 
4. `char* getCurrentBranch()`: lit le fichier caché `.current_branch` pour retourner le nom de la branche courante
5. `void printBranch(char* branch)`: parcourt la branche `branch` + pour chacun de ses commits, affiche son hash et son message descriptif *(s'il en existe un)*
6. `List* branchList(char* branch)`: construit + retourne une liste chainée contenant le hash de tous les commits de la branche
	> Pour parcourir une branche: clef `predecessor`

7. `List* getAllCommits()`: renvoie la liste des hash des commits de toutes les branches *(sans répétition)*

## SIMULATION DE `GIT CHECKOUT`
`git checkout` permet de naviguer entre les branches. Quand `checkout` est appelée avec un nom de branche, elle permet de changer de branche.

### Simulation de `git checkout`  
```C
void myGitCheckoutBranch(char* branch);
```
- modifie `.current_branch` pour contenir le **nom de la branche donné en paramètre**
- modifie `HEAD` pour contenir le **hash du dernier commit** de `branch` *(contenu dans le même fichier)*
- restaure le `WorkTree` correspondant au dernier commit de `branch`

### Avantage de `git checkout`
- peut être appelée avec le hash de n'importe quel commit, permettant à l'utilisateur de retourner sur n'importe quelle version de son projet. 
- permet à l'utilisateur de doner que les premiers caractères du hash d'un commit *(pour ne pas avoir à le taper entièrement au clavier)*
#### Simulation 
```C
void myGitCheckoutCommit(char* pattern);
```
- récupère la liste de tous les commits existants
- filtre cette liste pour ne garder que ceux qui **commencent** par `pattern`
	- s'il ne reste plus qu'**un** hash: met à jour la référence `HEAD` pour pointer sur ce hash + restaure le `WorkTree` correspondant
	- s'il ne reste plus **aucun** hash: affiche le message d'erreur à l'utilisateur
	- s'il reste plusieurs hash: affiche tous + demande à l'utilisateur de préciser sa requête

### Side function
- `void restoreCommit(char* hash_commit)`: restaurer le `WorkTree` associé à un commit dont le hash est donnée en paramètre
- `List* filterList(List* L, char* pattern)`: retourne une nouvelle liste contenant uniquement les éléments de `L` **commencant** par `pattern`