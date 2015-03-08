ALGORITHME
----------

aperçu bref des fonctions de programme.c

printf avec des commentaires sur la manière de jouer
scanf pour demander ce que veut le joueur:

1/if(scanf=="exit") then return 0

2/if(scanf=="game") then new_game() print_board()
printf avec le tableau de jeu + commentaire pour introduire un move
scanf des coordonnées
while(!ismoveseqvalid(coord)) //on a oublié un cas : celui ou le joueur donne des coordonnées d'une piece blanche ou d'une acse vide
printf erreur entrer les coordonnees sous le bons format cité ci-dessous
//end while
apply_move() on board and print_board()
current_player = !current_player //cela fonctionne si on utilise 0 et 1 pour les joueurs
//j'ai oublié le changement de joueur possible dans undo_moves

3/[else return -1]

//end

on met tout ça dans une grande boucle pour refaire chaque tour des deux joueurs et on peut rajouter(optionnel) une
autre boucle encore pour redemander si le joueur veut rejouer aux dames.
