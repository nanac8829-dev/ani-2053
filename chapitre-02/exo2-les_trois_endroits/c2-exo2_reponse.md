# Exercice 2

Il est question ici de modifier un fichier et de donner le resultat.

le fichier modifié est le fichier 1. Pour le modifier, j'ai ouvert le dossier depot_vide sur vs code, ouvert le fichier 1 puis je l'ai modifié. Après modification, j'ai ouvert le terminal de vs code et j'ai tapé les commandes suivantes:
```
git status (Après la modification)
git add .
git status
git commit -m "Modification du fichier 1"
git status
```
# git status après la modification
```
On branch master
Your branch is ahead of 'origin/master' by 2 commits.
  (use "git push" to publish your local commits)

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
        modified:   fichier1.txt

no changes added to commit (use "git add" and/or "git commit -a")
```
# git add fichier1.txt
N'a pas de retour.
# git status après le git add fichier1.txt
```
On branch master
Your branch is ahead of 'origin/master' by 2 commits.
  (use "git push" to publish your local commits)

Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
        modified:   fichier1.txt
```
# git commit
```
[master 99b14c7] Modification du fichier 1
 1 file changed, 2 insertions(+), 1 deletion(-)
 ```
# git status après le commit
```
On branch master
Your branch is ahead of 'origin/master' by 3 commits.
  (use "git push" to publish your local commits)

nothing to commit, working tree clean
```
# Ce qui change
Le git status après la modification nous informe que le fichier 1 a été modifié mais la modfication n'a pas encore été enregistrée.

Celui après le git add dit que la modification a été localisée et n'attend plus qu'à etre enregistrée.

Dans le dernier git status (après le commit) on nous dit que toutes les modifications ont été enregistrées.