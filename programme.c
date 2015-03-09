#include <stdio.h>
#include <stdlib.h>
#include "dames.c"


int main(int argc, int *argv[]){
    begining:
printf("Bienvenue dans Jeu de Dames V1.0\n");
printf("Que souhaitez-vous faire?\n");
printf("Exit(x) / Play(p) / Secret options(s)\n"); // mettre dans secret options la taille du tableau?
int temp;
int res;
res = scanf("%c", &temp); //erreur possible
if(res!=1 || temp=='x')
{
	printf("A bientôt!");
	\\delay end of game
	return 0;
}
if(temp=='s')
{
	//afficher les options
	goto begining; //occasion d'utiliser un goto
}
else
{
	int tab_size;
	printf("Quel est la taille du plateau : ");
	scanf("%d",&tab_size); //erreur possible
	struct game * jeu=(struct game *) malloc(sizeof(game));
	jeu = new_game(tab_size, tab_size);
	print_board(jeu);
	while(//le jeu n'est pas gagné par qqun)
	{
		printf("C'est au tour du joueur %s\n", ); //je ne trouve pas de moyen simple d'afficher la couleur du joueur
		struct move play = {NULL,NULL};
		struct move_seq * iter = play.seq;
		struct move_seq prev={NULL,NULL,NULL,0b00000001,NULL,0}; //init pour passer dans la boucle
		while(prev.piece_value!=EMPTY)
		{
			struct move_seq cur_play;
			do
			{
			if(!is_move_seq_valid(jeu, &cur_play, &prev, NULL))
			{
				printf("Erreur : votre mouvement n'est pas légal. Veuillez introduire un autre mouvement.\n");
			}
			printf("VOTRE MOUVEMENT\n ---------------\n Position de la pièce à bouger(écrit sous la forme (X,Y)): ");
			int x1;
			int y1;
			scanf("(%d,%d)",&x1,&y1); //erreur possible
			struct coord old_coord = {x1,y1};
			printf("Nouvelle position de cette pièce(écrit sous la forme (X,Y): ");
			int x2;
			int y2;
			scanf("(%d,%d)",&x2,&y2); //erreur possible
			struct coord new_coord = {x2,y2};
			cur_play = {NULL,old_coord,new_coord,0,NULL,game->board[x1][y1]};
			}
			while(!is_move_seq_valid(jeu, &cur_play, &prev, NULL));
			prev = cur_play; // le mvt en cours devient le précédent
			*iter = cur_play; // on ajoute la séquence
			iter=iter.next; // et on avance pour le suivant
		}
		free(iter); // plus besoin de iter
		//le move est construit et se trouve dans la variable play
		apply_moves(jeu,play);
		print_board(jeu);
	}
}

    //struct game *Jeu = new_game(10,10);//set up game
    //print_board(Jeu); // show game
    //struct coord old = {1,3};
    //struct coord newC = {2,4};
    //struct move_seq f ={NULL,old,newC,0,NULL,0};
    //struct move mv = {NULL,&f};
    //struct coord ta;
    //int r =  is_move_seq_valid(Jeu,&f,NULL,&ta);
    //printf("%d",r);
    //free_game(Jeu); // stop and free memory from game
    
    // check validity of move_seqs in moves
    //struct move_seq *iter_seq = (struct move_seq*) malloc(sizeof(struct move_seq *));
    //iter_seq = game->moves->seq;
    //struct move_seq *prev = (struct move_seq) calloc(sizeof(move_seq));
    //struct coord taken;
    //if (taken == NULL) {
    //    return -1;
//    }
    //while (iter_seq != NULL)
    //{
        //int valid = is_move_seq_valid(game,iter_seq,prev,taken);
        //if (valid == 0 ) {
        //    return -1; // ERREUR du mouvement ou autre
        //} else if (valid == 2) {
                //capture
        //        game->board[iter_seq->c_new.x][iter_seq->c_new.y] = game->board[iter_seq->c_old.x][iter_seq->c_old.y]; // move old pawn position to new
        //        game->board[iter_seq->c_old.x][iter_seq->c_old.y] = EMPTY;
        //        game->board[taken.x][taken.y] = EMPTY; // remove taken pawn



        //} else {
            // simple deplacement
        //    game->board[iter_seq->c_new.x][iter_seq->c_new.y] = game->board[iter_seq->c_old.x][iter_seq->c_old.y]; // move old pawn position to new
        //        game->board[iter_seq->c_old.x][iter_seq->c_old.y] = EMPTY;
        //    return 1;
        //}
        //prev = iter_seq;
        //iter=iter->next;
    //}
}
