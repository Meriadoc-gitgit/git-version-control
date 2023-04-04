# GESTION DES COMMITS
## Préliminaire
Pour pouvoir suivre l'évolution d'un projet, il faut pouvoir organiser chronologiquement différents enregistrements instantanés. 

Pour cela, on utilise ce qu'on appelle des *commits*, qui sont des enregistrements instantanés associés à des étapes jugées importantes dans la chronologie du projet.
>**Commits** = point de sauvegarde

>*On souhaite garder une trace quand on s'intérresse à l'évolution du projet.*

Avec notre implémentation: 
- Un commit est associé à l'enregistrement instantané d'un `WorkTree`
- *[...]* accompagné d'autres informations relatives au point de sauvegarde

### Structure utilisée
```c
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
```
#### Note
- Un commit est implémenté par une table de hachage dont les clés et les valeurs sont des chaines de caractères
- Les éléments de la table doivent correspondre aux informations associées au point de sauvegarde
- Un `Commit` **devra au moins** contenir une paire de la forme `("tree",hash)` 
	
	où `hash` est le hash du fichier correspondant à l'enregistrement instantané d'un `WorkTree`

> **Remarque:** Cette partie a pour but de simuler les commandes `git add` et `git commit` de manière simplifiée

## Fonction HASH choisie: `DJB2`
- [Homepage link](http://www.cse.yorku.ca/~oz/hash.html)
- [DJB2 Algo explained](https://thealgorithms.github.io/C/d4/de3/hash__djb2_8c.html#details)
![Dependency graph for hash_djb2.c](sub_img/djb2.png)
- [Hash Functions for Strings](https://youtu.be/jtMwp0FqEcg)

## Fonctions de base
1. `kvp* createKeyVal(char* key,char* val)` : allouer + initialiser un élément
	
    `void freeKeyVal(kvp* kv)` : libérer la memoire associé à un élément
2. `char* kvts(kvp* k)`: convertir un élément en une chaine de caractères de la forme `cle:val`
	
    `kvp* stkv(char* str)`: permet de faire la conversion inverse
3. `Commit* initCommit()`: allouer et initialiser un `Commit` de taille fixée *(donnée par une constante du programme)*
4. `void commitSet(Commit* c,char* key,char* value)`: insère la paire `(key,value)` dans la table, en gérant les collisions par adressage ouvert et probing **linéaire**