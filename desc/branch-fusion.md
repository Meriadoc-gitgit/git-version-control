# Gestion des fusions de branches
`git merge` permet de fusionner 2 branches: créer un nouveau commit dont le `WorkTree` correspond à la fusion des `WorkTree` des derniers commits de ces branches

>**Note**: Ce nouveau commit est un peu spécial car il possède 2 prédécesseurs au lieu d'un seul

**Risque de conflits**: Un conflit survient lorsque les worktrees à fusionner contiennent un fichier/répertoire de même nom, mais avec des hash *(donc des contenus)*