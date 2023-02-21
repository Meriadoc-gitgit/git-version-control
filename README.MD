# Projet: Réalisation d'un logiciel de gestion de versions
> **Aim:** Realisation d'un *outil de suivi et de versionnage de code* (**`git`**)
## Cadre du projet
Un logiciel de **gestion de versions** est un outil permettant le stockage, le suivi et la gestion de plusieurs versions d'un projet *(ou d'un ensembles de fichiers)*. 
- offrent un **acces aise a l'historique de toutes les modifications effectuees** sur les fichiers + permettent notamment de *recuperer une version anterieure en cas de probleme*. 
- tres utiles dans le cadre de travail collaboratif, permettant de *fusionner de maniere intelligente differentes versions d'un meme projet*. 
> **Objectif:** etudier le fonctionnement d'un logiciel de gestion de versions, *en detaillant* differentes structures de donnees impliquees dans sa mise en oeuvre. 
## Vers la creation d'enregistrements instantanes
Sous `git`, tous les objets, *qu'ils soient relatifs aux fichiers versionnes ou a leurs meta-donnees*, sont enregistres sous forme de fichiers. <br>
Ces fichiers ont pour particularite de pouvoir deriver le chemin ou ils sont stockes a partir de leur contenu. 
<p align="center"><img src="fichiers-stocke.png"></p>

**Note:** 
- Ici, la fonction de hachage `SHA256` est appliquee sur le contenu du fichier
- Le chemin ou doit etre stocke le fichier est obtenu en inserant `/` entre le 2e et 3e caracteres du hash. 

Faire dependre le chemin du contenu permet notamment de sauvegarder toutes les differentes versions du fichier. 


**Mofifier le contenu du fichier** va: 
- modifier le chemin vers lequel le sauvegarder 
- creer differentes sauvegardes correspondant a differentes etats du fichier. 

*(Quand on realise une telle sauvegarde, on dit communement qu'on "enregistre un instantane"/qu'on creer un "enregistrement instantane".)*
***
## Prise en main de Bash
Les donnees sur le disque sont stockees dans des fichiers, dont des structures de donnees qui apparaissent aux programmes comme des **suites finies d'octets**. 
> **File system**: 
>- La structure de donnees qui organise les fichiers sur le disque
>- Une des fonctionnalites principales d'un systeme d'exploitation

Bash permet d'utiliser une interface en ligne de commande.
### Liste de commandes basiques en Bash
- `man`: afficher la documentation de la commande concernee
- `pwd`: afficher le repertoire courant
- `cd`: se positionner sur un certain repertoire de travail
- `mkdir`: creer un repertoire
- `touch` *`nom_de_fichier`*: creer le fichier de nom *`nom_de_fichier`*
- `ls`: lister les fichiers d'un repertoire
- `cat`: affichier le contenu d'un fichier
- `rm`: supprimer un fichier

**Other:** 
- possible de rediriger une des sorties vers un fichier. <br>
  `ls > list.txt`: obtenir `list.txt` contenant la liste des fichiers et repertoires presents dans le repertoire courant *(plutot creer `list.txt`)*
- possible de rediriger la sortie d'une commande vers une autre, en utilisant **pipeline**. <br>
`cat noms.txt | sort`: lire la liste de noms presente dans le fichier `noms.txt` + la transmettre a la fonction `sort` qui va la trier *(ordre alphabetique)*

Linux met en place des flux globaux permettant a des commandes appelees, de dialoguer avec le programme appelant en ecrivant ou en lisant depuis ces flux.

**Les flux globaux:** 
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

**Note:**
- **Commande `sha256sum`:** hacher le contenu d'un fichier en utilisant la fonction de hachage `SHA256`.
- **Fonction `system`:** permet de faire des appels systeme. 