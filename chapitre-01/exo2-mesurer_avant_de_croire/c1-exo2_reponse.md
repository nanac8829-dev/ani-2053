A la fin du compte, on obtient:
4337 fichiers .cpp et .h,
1342559 lignes de code,
207 fichiers de projet .jenga.

# Comparaison

# fichiers .cpp et .h 
Afin de determiner le nombre de fichiers .cppet .h, j'ai utiliser les commandes suivantes
(Get-ChildItem -path . -Filter *.cpp -Recurse | Measure-Object).Count
(Get-ChildItem -path . -Filter *.h -Recurse | Measure-Object).Count 
4337>2641.   
# lors de la comparaison, on constate que l'ecart est tres grand entre le nombre de fichiers trouvés et ceux contenus dans le chapitre

# Les autres fichiers
On a 60 fichiers .hpp et 311 fichiers .c.
# Nombre de lignes de code
ici j'ai utilisé codeline counter et j'ai obtenu 1342559>1193385.

# Nombre de fichiers de projet .jenga
J'ai utilisé la commannde (Get-ChildItem -path . -Filter *.jenga -Recurse | Measure-Object).Count ce qui a donné 207<221.