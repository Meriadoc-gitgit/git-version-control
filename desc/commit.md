# GESTION DES COMMITS
## Preliminaire
Pour pouvoir suivre l'evolution d'un projet, il faut pouvoir organiser chronologiquement differents enregistrements instantanes. 

Pour cela, on utilise ce qu'on appelle des *commits*, qui sont des enregistrements instantanes associes a des etapes jugees importantes dans la chronologie du projet.
>**Commits** = point de sauvegarde

>*On souhaite garder une trace quand on s'interresse a l'evolution du projet.*

Avec notre implementation: 
- Un commit est associe a l'enregistrement instantane d'un `WorkTree`
- *[...]* accompagne d'autres informations relatives au point de sauvegarde

### Structure utilisee
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
- Un commit est implemente par une table de hachage dont les cles et les valeurs sont des chaines de caracteres
- Les elements de la table doivent correspondre ayx informations associees au point de sauvegarde
- Un `Commit` devra au moins contenir une paire de la forme `("tree",hash)` 

  où `hash` est le hash du fichier correspondant a l'enregistrement instantane d'un `WorkTree`
> **Remarque:** Cette partie a pour but de simuler les commandes `git add` et `git commit` de maniere simplifiee