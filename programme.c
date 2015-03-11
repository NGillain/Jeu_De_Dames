#include <stdio.h>
#include <stdlib.h>
#include "dames.c"


int main(int argc, int *argv[])
{
	begining:
    printf("Welcome to the Great Game of Checkers V3.1.1\n");
    printf("");
    printf("What would you like to do?\n");
    printf("Exit(x) / Play(p) / Options(o)\n"); // mettre dans secret options la taille du tableau?
    char temp;
    int res; //useful variable for results of all scanf call
    res = scanf("%c", &temp);
    while(res!=1)
    {
		printf("You didn't enter a character or a wrong one. Please enter only one character in the list here under according to what you want to do\n");
		printf("Play(p) / Options(O) / Exit(x)\n");
		res = scanf("%c", &temp);
	}
    if(temp=='x')
    {
        printf("See you soon! \n");
        return 0;
    }
    else if(temp=='s')
    {
		printf("OPTIONS\n-------\n\n");
		printf("Nothing for now sry\n");
		goto begining;
	}
	else if(temp=='p')
	{	
		int tab_size;
		printf("What's the size of the game board (max 15 lines, min 4 lines)? ");
		res = scanf("%d",&tab_size); //erreur possible
		while(res!=1)
		{
			printf("You didn't enter a number. Please enter a number of lines between 4 and 15 : ");
			res = scanf("%c", &tab_size);
		}
		if(tab_size > 15)
		{
			tab_size=15; // no more than 15 lines
		}
		if(tab_size < 4)
		{
			tab_size=10; // short boards are so easy I propose you to play on a traditional one :)
		}
		struct game *jeu = new_game(tab_size, tab_size);
		printf("\n");
		print_board(jeu);
		printf("\n");
		printf("Here are some explanations : the horizontal axis is called X and the vertical one Y.\n");
		printf("Illegal moves will be detected and you'll have an other chance to give it. Please follow carefully the given format!\n");
		printf("\n");
		while (nb_black>0 || nb_white>0)
		{
			struct move *mv = (struct move *) malloc(sizeof(struct move));
			struct move_seq *seq = (struct move_seq *) malloc(sizeof(struct move_seq));
			printf("It is the %d player's turn\n",jeu->cur_player);
			printf("1(w) is white and 0(b) is black\n");
			printf("\n");
			printf("YOUR MOVE :\n -----------\nPosition of the piece to move(written as follow X,Y) : ");
			int x1;
			int y1;
			res = scanf("%d,%d",&x1,&y1);
			while(res!=2)
			{
				printf("The format was wrong. Try again and use the following format X,Y : ");
				res = scanf("%d,%d",&x1,&y1);
			}
			struct coord old_coord = {x1,y1};
			printf("valeur ancienne piece : %d\n",jeu->board[x1][y1]);
			printf("Nouvelle position de cette pièce(écrit sous la forme (X,Y): ");
			int x2,y2;
			scanf("%d,%d",&x2,&y2); //erreur possible
			struct coord new_coord = {x2,y2};
			seq->c_new = new_coord;
			seq->c_old = old_coord;
			seq->old_orig = jeu->board[x1][y1];
			seq->next = NULL;
			seq->piece_value = 0b00000000;
			mv->seq = seq;
			mv->next = NULL;
			int result = apply_moves(jeu,mv);
			if (result == -1)
			{
				printf("Your move is incorrect : %d. Try again\n", result);
			}
			else
			{
				printf("Your move is correct : %d.\n", result);
			}
			printf("Here is the game board : \n\n");
			print_board(jeu);
			printf("\n");
		}
		free_game(jeu);
	}
}
