# Rapport de projet: Git-version-control
>Réalisation d'un *outil de suivi et de versionnage de code* (**`git`**)

>**Current progress**: 100% finished

# Préliminaire
## Highlight
### Information principale
- Nom de projet: Git-version-control
- Lien Github du projet: https://github.com/Meriadoc-gitgit/GIT-version-control.git
- Instructeurs: Paul Hermouet & Louis Serrano
## Cadre de projet
>**Objectif**: Étudier le fonctionnement d'un logiciel de gestion de versions, *en détaillant* différentes structures de données impliquées dans sa mise en oeuvre. 

Un logiciel de **gestion de versions** est un outil permettant le stockage, le suivi et la gestion de plusieurs versions d'un projet *(ou d'un ensembles de fichiers)*. 
- offrent un **accès aisé à l'historique de toutes les modifications effectuées** sur les fichiers + permettent notamment de *récupérer une version antérieure en cas de problème*. 
- très utiles dans le cadre de travail collaboratif, permettant de *fusionner de manière intelligente différentes versions d'un même projet*. 

## Crew member
1. **Vu Hoang Thuy Duong**
	- Numéro d'étudiant: 21110221
	- Groupe: MONO 6
	- Titre: MVP d'algorithme 
	- Pseudo: authentic vietnamese 
	- LinkedIn: https://www.linkedin.com/in/meriadocdgv/
	- Github: https://github.com/Meriadoc-gitgit

2. **Halimatou Diallo**
	- Numéro d'étudiant: 
	- Groupe: MONO 6
	- Titre: roi de valgrind *(vaincre toute sorte de data leak)*
	- Pseudo: 
	- LinkedIn: 
	- Github: 

## Pré-requis
1. Connaissances de base des commandes en Bash pour la manipulation du système: `mkdir`, `rm`...
2. Connaissances aisées en langage C *(projet programmé 100% en C)*
3. Connaissances de base en Structures des données
	- Algorithme importante implémentée: 
		- Algorithme de hachage
		- Gestion de collisions
		- Manipulation d'une structure arborescente
		- ...
	- Cours suivi: LU2IN006 - Structure des données | Sorbonne Université | 2022-2023
4. Exigence secondaire: extra autonomie, bonne gestion de stress + extra sens de team-player *(tout au long du projet)*

## Configuration *(pour MacOS)*

---
# Manuel d'usage
## Choix de fonction de hachage
>Algorithme choisie: **DJB2**

### Implémentation
```C
int hash(char* str) {
	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}
```
### Pourquoi un tel choix ? 
L'explication est simple. En comparant avec les 2 autres algorithmes fournies *(dont SBDM et LOSE LOSE)*, DJB2 est une algorithme renommée et certifiée en tant que meilleure algorithme pour les chaines de caractères par une grande population *(avis de l'auteur du site + de la grande communauté des developpers lus sur Stackoverflow et d'autres forums)*. 

L'utilisation de DJB2 garantit une excellente distribution et vitesse sur de nombreux jeux de clés et tailles de table différents. Pourtant, la magie du nombre 33 *(pourquoi il fonctionne mieux que de nombreuses autres constantes, premières ou non)* n'a jamais été suffisamment expliquée.

>**Note**: Le résultat de cette fonction peut notamment être négative. Afin d'assurer une bonne insertion des couples `(key,value)` dans `Commit`, il est forcément recommandé d'utiliser la valeur absolue du `hash` renvoyé par DJB2.

### Changement par rapport à l'algorithme fournie
1. Changement de type du paramètre en entrée
	
	Suivant l'algorithme fournie sur le site web mentionné dans l'énoncé, le type du paramètre pris en entrée sera un `unsigned char`. Pourtant, en testant cette fonction avec un tel paramètre, la boucle risque de s'arrêter immédiatement une fois parcouru le premier caractère du chaine en entrée au lieu d'avancer jusqu'à son dernier élément. 
	
	Cela cause donc un grand problème de collision, vu que les résultats de hachage ne seront que les hachages des caractères à la place des chaines de caractères. Donc, pour améliorer la complexité du programme et meilleure gestion de collisions, nous avons décider de modifier ce paramètre en `char*`

2. Changement de type retour de la fonction
	Pour simplifier la compréhension du programme, nous avons modifier le type de retour de cette fonction en `int` au lieu de `unsigned long`. En terme de résultat, après plusieurs assertions, nous trouve que cela ne cause pas de problème à la simulation de l'ensemble du logiciel.

## Commandes `myGit` simulées
### Quelques notations importantes 
Pour assurer une bonne poursuite tout au long du projet, nous notons ici quelques commentaires brefs et importants. 
- Nom du logiciel: `myGit` *(a.k.a. `git` version Sorbonne)*
- Concernant les commandes, leurs noms et leurs fonctionnalités restent les mêmes que ceux de `git`
- Afin de gérer la simulation de `myGit` de manière efficace avec une bonne complexité, une fonction s'appellant `myGit_casebreak` a été écrit. 
	
	Tout au long du programme, basant sur les arguments en entrée de `myGit`, cette fonction applique notre algorithme de hachage implémentée afin de diviser effectivement les 14 cas pour 14 commandes. Comme ça, dans `myGit.c`, nous ne devons plus faire l'appel à `strcmp`, mais pouvoir utiliser l'instruction `switch case-break`. 
	
### Fonction de base 
1. `blob[File | WorkTree | Commit]` 
2. `saveWorkTree`
3. `restoreWorkTree`

### Simulation de `myGit`
```Bash
./myGit <command> [<option> <arg1> <arg2> ...]
```
- `command`: commande indiquant l'action voulue de l'utilisateur, à choisir à partir de la liste ci-dessous
- `option`: facultatif, *`-m`, `-f`, etc.*
- `arg1`, `arg2`...: facultatif selon `command` utilisée, indiquant les données nécessaires pour la simulation des commandes
### Liste de commandes
Cette liste est composée de **14 commandes primordiales** du logiciel simulées tout au long de ce projet semestriel.
1. `./myGit init`: initialise le répertoire de références
2. `./myGit list-refs`: affiche toutes les références existantes
3. `./myGit create-ref <name> <hash>`: crée la référence `<name>` qui pointe vers le commit correspondant au `<hash>` donné
4. `./myGit delete <name>`: supprime la référence `name`
5. `./myGit add <elem> [<elem2> <elem3> ...]`: ajoute un ou plusieurs fichiers/répertoires à la zone de préparation _(prochain commit)_
6. `./myGit list-add`: affiche le contenu de la zone de préparation
7. `./myGit clear-add`: vide la zone de préparation
8. `./myGit commit <branch-name> [-m <message>]`: effedctue un commit sur une branche, avec ou sans mesage descriptif
9. `./myGit get-current-branch` : affiche le nom de la branche courante
10. `./myGit branch <branch-name>`: crée une branche qui s'appelle `<branch-name>` si elle n'existe pas déjà
11. `./myGit branch-print <branch-name>`: affiche le hash de tous les commits de la branche, accompagné de leur message descriptif éventuel
12. `./myGit checkout-branch <branch-name>`: réalise un déplacement sur la branche `<branch-name>`
13. `./myGit checkout-commit <pattern>`: réalise un déplacement sur le commit qui commence par `<pattern>`
14. `./myGit merge <branch> <message>`: merge la branche courante avec la branche `<branch>` en gérant les conflits

# Structure de projet


# Analyse de Warning
>Ainsi déjà bien gérés avant le rendu de projet
## Concernant les Data Leak
### Gestion de valgrind
### Lors des tests
## Concernant les variables redondant
## Concernant le mal-fonctionnement commandes


---

## Conclusion et Remerciement
