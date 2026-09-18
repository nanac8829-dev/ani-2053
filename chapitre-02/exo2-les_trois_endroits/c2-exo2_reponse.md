# Exercice 2

Il est question ici de modifier un fichier et de donner le resultat.

le fichier modifié est le fichier 1. Pour le modifier, j'ai ouvert le dossier depot_vide sur vs code, ouvert le fichier 1 puis je l'ai modifié. Après modification, j'ai ouvert le terminal de vs code et j'ai tapé les commandes suivantes:
```
git add .
git commit -m "Modification du fichier 1"
git status
```
# git add .

git add . n'a pas presenté de retour.

# git commit -m "Modification du fichier 1"

le retour du commit est:
```
[master 3c48443] Modification du fichier 1
 1 file changed, 0 insertions(+), 0 deletions(-)
```

# git status

le retour est:
```
On branch master
Your branch is ahead of 'origin/master' by 1 commit.
  (use "git push" to publish your local commits)

nothing to commit, working tree clean
```

# Ce qui change
git add . n'a pas de retour. 
git commit -m "Modification du fichier 1" quant à lui donne un nom à la modification du fichier qui a été faite.
git status lui affiche l'état du travail.