# Projet: Réalisation d'un logiciel de gestion de versions
**Aim:** Réalisation d'un *outil de suivi et de versionnage de code* (**`git`**)

>**Current progress**: 90% finished

>**Réalisatrices**: Vu Hoang Thuy Duong & Halimatou Diallo

>**Instructeurs**: Paul Hermouet & Louis 

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
- `./myGit init`: initialise le répertoire de références
- `./myGit list-refs`: affiche toutes les références existantes
- `./myGit create-ref <name> <hash>`: crée la référence `<name>` qui pointe vers le commit correspondant au `<hash>` donné
- `./myGit delete <name>`: supprime la référence `name`
- `./myGit add <elem> [<elem2> <elem3> ...]`: ajoute un ou plusieurs fichiers/répertoires à la zone de préparation *(prochain commit)*
- `./myGit list-add`: affiche le contenu de la zone de préparation
- `./myGit clear-add`: vide la zone de préparation
- `./myGit commit <branch-name> [-m <message>]`: effectue un commit sur une branche, avec ou sans message descriptif
