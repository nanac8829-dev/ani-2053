L'exercice consiste à créez un dépôt vide, ajoutez trois fichiers en trois commits, et affichez l'historique en une ligne par commit. Puis affichez le graphe.

Tout d'abord on a la creation du depot vide (depot_vide).
Ensuite pour la creation des fichiers j'ai utilisé le terminal de la machine.
# creation du premier fichier et direction vers le depot github

commandes utilisées
```
 echo "Bonjour" > fichier1.txt
 git add fichier1.txt
 git commit -m "Ajout du fichier 1"
 git push
```
# creation du deuxieme fichier et direction vers le depot github

commandes utilisées
```
 echo "Salut" > fichier2.txt
 git add fichier2.txt
 git commit -m "Ajout du fichier 2"
 git push
```
# creation du troisieme fichier et direction vers le depot github

commandes utilisées
```
 echo "Bonsoir" > fichier3.txt
 git add fichier3.txt
 git commit -m "Ajout du fichier 3"
 git push
```
Pour afficher l'historique en une ligne par commit j'ai utilisé la commande 
"git log --oneline"

# le resultat est :
```
PS C:\Users\User\desktop\depot_vide> git log --oneline
81a186f (HEAD -> master, origin/master) Ajout du fichier 3
f3d1ac0 Ajout du fichier 2
0817df9 Ajout du fichier 1
```
Pour le graphe j'ai utilisé 
"git log --graph"

# le resultat est :
```
PS C:\Users\User\desktop\depot_vide> git log --graph
* commit 81a186fe882690505f78f23a00e103a338f5a6e0 (HEAD -> master, origin/master)
| Author: nanac8829-dev <nanac8829@gmail.com>
| Date:   Fri Sep 18 15:21:17 2026 +0100
|
|     Ajout du fichier 3
|
* commit f3d1ac0893a515a1cf19c5c83740ea969d064862
| Author: nanac8829-dev <nanac8829@gmail.com>
| Date:   Fri Sep 18 15:20:06 2026 +0100
|
|     Ajout du fichier 2
|
* commit 0817df96c3be2ea3472e31f4c43e408188728052
  Author: nanac8829-dev <nanac8829@gmail.com>
  Date:   Fri Sep 18 15:17:09 2026 +0100

      Ajout du fichier 1
```