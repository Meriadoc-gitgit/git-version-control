# Projet: Réalisation d'un logiciel de gestion de versions
**Aim:** Réalisation d'un *outil de suivi et de versionnage de code* (**`git`**)

>**Current progress**: 90% finished

>**Réalisatrices**: Vu Hoang Thuy Duong & Halimatou Diallo

>**Instructeurs**: Paul Hermouet & Louis Serrano

# Cadre du projet
Un logiciel de **gestion de versions** est un outil permettant le stockage, le suivi et la gestion de plusieurs versions d'un projet *(ou d'un ensembles de fichiers)*. 
- offrent un **accès aisé à l'historique de toutes les modifications effectuées** sur les fichiers + permettent notamment de *récupérer une version antérieure en cas de problème*. 
- très utiles dans le cadre de travail collaboratif, permettant de *fusionner de manière intelligente différentes versions d'un même projet*. 

**Objectif:** Étudier le fonctionnement d'un logiciel de gestion de versions, *en détaillant* différentes structures de données impliquées dans sa mise en oeuvre. 
## Path
1. [Preliminary](desc/preliminary.md)
2. [Enregistrement de plusieurs instantanées](desc/multi_instant.md)
3. [Gestion des commits](desc/commit.md)
4. [Timeline arborescente](desc/timeline.md)
5. [Fusions de branches](desc/branch-fusion.md)

## `myGit` command simulated
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
