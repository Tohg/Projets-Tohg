# Analyse Mathématique

### Analyse des symétries.
On peut faire une rotation en positionnant le cavalier dans les autres coins et en faisant les déplacement par leurs miroir sur la diagonale par rapport au cavalier


### Etudes des cas impossibles : 
Pour faire un cycle hamiltonien, il faut visiter toutes les cases une fois et une seule.

- 3x3 -> sur ce plateau si l’on pose le cavalier sur le centre (soit en B2), le cavalier aucun chemin de sortie, celles-ci se déroulerait en dehors du plateau. Le graphe n’est pas connexe et donc impossible à réaliser pour un parcours hamiltonien

- 4x4 -> Un plateau de 4x4 a 16 cases dont les coins sont de degré 2.

    Problème : avec 4 coins de degré 2, on “verrouille” des cases qui doivent obligatoirement être visitées quand cela est possible. 
On est déjà bloqués avec 2 coins qui sont dans le cycle car chacun d’entres eux exigent d’être visités à la suite de la case de “sortie” du coin en diagonale.

    De cette manière il n’est pas possible de refermer le cycle.

    Mais même sans ça, si nous essayons de nous échapper de cette boucle en commençant par un coin, cela ne fera que retarder le fait que nous nous bloquerons dans un autre coin car l’on aura pénétré sur une autre case centrale qui nous oblige à rentrer dans l’autre boucle sans finir les cases sur les bords
