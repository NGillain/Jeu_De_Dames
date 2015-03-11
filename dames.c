#include <stdio.h>
#include <stdlib.h>
#include <math.h> //must be compiled with -lm for the linker
#include "dames.h"

#define PION_NOIR   0b00000001
#define PION_BLANC  0b00000101
#define EMPTY       0b00000000

int nb_white;
int nb_black;
int moves_done;
int simple_moves;

struct game *new_game(int xsize, int ysize)
{
    //spec : create a new struct representing a game and return it
    struct game *Newgame = (struct game *) (malloc(sizeof(struct game)));
    if (Newgame == NULL)
    {
        return NULL; //run out of memory
    }
    Newgame->xsize = xsize;
    Newgame->ysize = ysize;
    Newgame->cur_player = PLAYER_WHITE; // the white pawns always begin
    Newgame->moves = NULL;
    Newgame->board = create_board(xsize,ysize); //init a new board
    if (Newgame->board == NULL)
    {
        return NULL;
    }
    return Newgame;
}

struct game *load_game(int xsize, int ysize, const int **board, int cur_player)
{
    //spec : load a game by initialization
    struct game *f = new_game(xsize,ysize); //malloc is in new_game
    f->board = board;
    f->cur_player = cur_player;
    return f;
}

void free_game(struct game *game)
{
	//spec : free all ressources used by a game structure
    for (int i=0; i<game->xsize; i++)
    {
        free(game->board[i]);
    }
    free(game->board);
    game->board = NULL;
    remove_moves(game,0);
    free(game);
    game=NULL;
    return;
}

int apply_moves(struct game *game, const struct move *moves)
{
    //spec : apply a move by adding it to the chained list, updating the game board and setting the current player to the opposite one
    struct move *iter_move = moves;
    while (iter_move != NULL)
    {
        struct move_seq *iter_seq; //iterator for move_seq
        iter_seq = iter_move->seq; //init of iter at the begining of the move
        struct coord taken;
        struct move_seq *prev = NULL; //hold the location of the previous move_seq
        while (iter_seq != NULL)
        {
            int valid = is_move_seq_valid(game,iter_seq,prev,&taken);
            int piece = game->board[iter_seq->c_old.x][iter_seq->c_old.y];
            if (valid == 0)
            {
                return -1; // invalid movement or other error
            }
            else
            {
                game->board[iter_seq->c_new.x][iter_seq->c_new.y] = piece; // move old pawn position to new
                game->board[iter_seq->c_old.x][iter_seq->c_old.y] = EMPTY;
                if (valid == 2)
                {
                    iter_seq->piece_value = game->board[taken.x][taken.y]; //update tne taken piece value
                    iter_seq->piece_taken.x = taken.x; //update the taken coordinates
                    iter_seq->piece_taken.y = taken.y;
                    iter_seq->old_orig = piece;
                    game->board[taken.x][taken.y] = EMPTY;
                    if(is_white(game->board[taken.x][taken.y]))
                    {
						nb_white--; //update the remaining white pieces
					}
					else if(is_black(game->board[taken.x][taken.y]))
					{
						nb_black--; //update the remaining black pieces
					}
                }
                if (is_white(piece) && (iter_seq->c_new.y == 0) )
                {
                    game->board[iter_seq->c_new.x][iter_seq->c_new.y] = upgrade(piece); //upgrade a white man into a white king

                }
                if (is_black(piece) && (iter_seq->c_new.y == (game->ysize-1))) {
                    game->board[iter_seq->c_new.x][iter_seq->c_new.y] = upgrade(piece); //upgrade a black man into a black king
               }
            }

            prev = iter_seq;
            iter_seq = iter_seq->next; //next!
        }
        game->cur_player = !game->cur_player; //other player's turn
        if (nb_black == 0)
        {
            game->cur_player = PLAYER_WHITE;
            return 1; // white player won
        }
        if (nb_white == 0)
        {
            game->cur_player = PLAYER_BLACK;
            return 1; // black player won
        }
        iter_move = iter_move->next;
    }
    // attention si capture final dans seq, cette historique ne sera pas enregistré car return avant !!!!!!!!!!!!!!!!!!!!!!!!
    struct move *iter_move_game = game->moves;
    while (iter_move_game != NULL)
    {
        iter_move_game = iter_move_game->next;
    }
    iter_move_game = moves;
    return 0;
}

void reverse_move(struct move **head)
{
	//spec : reverse the move linked list order
    struct move *prev = NULL;
    struct move *current = head;
    struct move *next;
    while (current != NULL)
    {
        next  = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}

void reverse_seq(struct move_seq **head)
{
	//spec : remove the seq linked list order
    struct move_seq *prev = NULL;
    struct move_seq *current = head;
    struct move_seq *next;
    while (current != NULL)
    {
        next  = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}

int upgrade(int value)
{
	//upgrade a man into a king
    return (value | 0b00000010);
}

int is_dame(int value)
{
    //spec : 0 if man or EMPTY and 1 if king
    return (((value << 6 ) >> 7) & 0b00000001);
}

int is_white(int value) // idem
{
    //spec : 0 if black or EMPTY and 1 if white
    return ( ((value >> 2) & 0b00000001) && (value != EMPTY));
}

int is_black(int value)
{   //spec : 0 if white or EMPTY and 1 if black
    return ( ((value >> 2) ^ 0b00000001) && (value != EMPTY));
}

int is_move_seq_valid(const struct game *game, const struct move_seq *seq, const struct move_seq *prev, struct coord *taken)
{
    //spec : check ONLY IF one single sequence is valid
    if ( (game->cur_player) != is_white(game->board[seq->c_old.x][seq->c_old.y]))
    {
        return 0; // if the piece and the player have different colors ==> move is illegal
    }
    int xold = seq->c_old.x;
    int yold = seq->c_old.y;
    int xnew = seq->c_new.x;
    int ynew = seq->c_new.y;
    if(abs(xold-xnew) != abs(yold-ynew))
    {
		return 0; //if nondiagonal displacement ==> move illegal
	}
    if (prev != NULL)  //if there is no previous move
    {
        if ((prev->c_new.x) != (seq->c_old.x)) //if the new position don't begin at the end position of the previous move ==> move is illegal
        {
            return 0;
        }
    }
    int c = is_white(game->board[xold][yold]);
    if (game->board[xold][yold] == EMPTY || game->board[xnew][ynew] != EMPTY)
    {
        return 0; // case où on veut aller non vide ou une case vide
    }
    if (0>xnew || xnew>(game->xsize) || 0>ynew || ynew>(game->ysize))
    {
        return 0; // illegal move : OutOfBound
    }
    int vec_X = (xnew-xold)/abs(xnew-xold); //vecteur unitaire du déplacement du pion/dame en x
    int vec_Y = (ynew-yold)/abs(ynew-yold); //vecteur unitaire du déplacement du pion/dame en y
    if (is_dame(game->board[xold][yold]))   // king moving
    {
        for(int a=1; a<abs(xnew-xold); a++)
        {
            if(c == is_white(game->board[xold+a*vec_X][yold+a*vec_Y]) && game->board[xold+a*vec_X][yold+a*vec_Y] != EMPTY) //Des qu'il y a un pion de la même couleur
            {
                return 0;
            }
        }
        int temp=0; //keep track of the number of opposite's color pawns
        struct coord t;
        for(int b=1; b<(xnew-xold); b++)
        {
            if(c != is_white(game->board[xold+b*vec_X][yold+b*vec_Y]))
            {
                temp++; // we found an opposite's color pawn
                t.x = xold+b*vec_X;
                t.y = yold+b*vec_Y;
            }
            if(temp>1) // more than one of these means illegal move
            {
                return 0;
            }
        }
        if(temp==0)
        {
            printf("Moving \n");
            return 1; // si rien le mvt est valide
        }
        //else if (seq->next == NULL && game->board[xnew+(xold-xnew)/(abs(xold-xnew))][ynew+(yold-ynew)/(abs(yold-ynew))] == EMPTY)
        //{
            //return 0; // si la dame finit son move et qu'elle se positionne qqpart où il n'y pas de pions avant
        //}
        else
        {
            taken->x = t.x;
            taken->y = t.y;
            return 2; //sinon c'est que le mvt est valide et la dame prend un pion
        }
    }
    else   // a man is moving
    {
        if (abs(xold-xnew) > 2) // men don't move further than 2
        {
            return 0;
        }
        if (c) // white men
        {
			printf("it is white\n");
            if (abs(xold-xnew) == 2) // if the man goes two step further
            {
				printf("it moves 2 steps\n");
				if(is_black(game->board[xold+vec_X][yold+vec_Y]))//check if there is an opposite color pawn
				{
					printf("la case entre est %d (0 si blanc et 1 si noir)\n", is_black(game->board[xold+(xnew-xold)/2][yold+(ynew-yold)/2]));
					taken->x = xold+vec_X;
					taken->y = yold+vec_Y;
					return 2;// if so, a white man took a black man
				}
				else
				{
					return 0; //else it is a trap
				}
            }
            if ((yold-ynew) < 0) // white men must move upward
            {
                return 0;
            }
        }
        else // black men
        {
            if (abs(xold-xnew) == 2)
            {
				if(is_white(game->board[xold+vec_X][yold+vec_Y]))
				{
					taken->x = xold+vec_X;
					taken->y = yold+vec_Y;
					return 2;//same as above
				}
				else
				{
					return 0;
				}
            }
            if ((yold-ynew) > 0) // black men must move downward
            {
                return 0;
            }
        }
    }
	printf("rattrapage au filet ==> MAUVAIS!\n ");
    return 1; // else without taking anything
}

int undo_moves(struct game *game, int n)
{
    reverse_move(game->moves);
    struct move *iter_move = game->moves;
    int undo_done = 0;
    while (iter_move != NULL && undo_done < n)
    {
        reverse_seq(iter_move->seq);
        struct move_seq *iter_seq = iter_move->seq;
        while(iter_seq != NULL)   // reset new coordinates to old ones and set the pawn back if where it was taken
        {
            game->board[iter_seq->c_old.x][iter_seq->c_old.y] = iter_seq->old_orig;
            game->board[iter_seq->c_new.x][iter_seq->c_new.y] = EMPTY;
            if (iter_seq->piece_value != EMPTY)
            {
                game->board[iter_seq->piece_taken.x][iter_seq->piece_taken.y] = iter_seq->piece_value;
            }
            iter_seq = iter_seq->next;
        }
        undo_done++;
        remove_moves(game,1);
        iter_move = iter_move->next;
        game->cur_player = !game->cur_player;
    }
    game->moves = iter_move; // plus besoin car ca se passe dans remove_move
    reverse_move(game->moves);
    return 0;
}
/*  remove_moves
*   Removes the first n moves from a chained list of moves and frees the memory from it
*   if n=0 then all element are removed
*/
void remove_moves(struct game *game,int n)
{
    struct move *iter_move = game->moves;
    struct move *iter_move_next = NULL;
    int removed = 0;
    if (n==0)
    {
        n = 10000;
    }
    while ( (iter_move != NULL) || (removed < n))
    {
        iter_move_next = iter_move->next;
        remove_move_seq(iter_move);
        free(iter_move);
        iter_move = iter_move_next;
        removed++;
    }
    game->moves = iter_move;
}
/*  remove_move_seq
*   Removes all move_seq from a move structure and frees the memory from it
*/
void remove_move_seq(struct move *move)
{
    struct move_seq *iter_seq = move->seq;
    struct move_seq *iter_seq_next;
    while (iter_seq != NULL)
    {
        iter_seq_next= iter_seq->next;
        free(iter_seq);
        iter_seq = iter_seq_next;
    }
    move->seq = NULL;
}

int create_board(int xsize, int ysize)
{
    int **board = (int **) malloc(xsize*sizeof(int*));
    if  (board == NULL)
    {
        return NULL;
    }
    //spec : initialize a game board with length x and height y
    int lines_to_fill;
    if ((ysize % 2) == 0)
    {
        lines_to_fill = (ysize-2)/2;
    }
    else
    {
        lines_to_fill = (ysize-1)/2; // let at least an empty line to play
    }
    if(lines_to_fill>5)
    {
        lines_to_fill = 5; // max 5 lines with men
    }
    for (int i=0; i<xsize; i++)
    {
        board[i] = (int *) malloc(ysize*sizeof(int));
        for (int j=0; j<ysize; j++)
        {

            if ( ((i+j)%2) == 1)   // si case à remplir et ...
            {
                if (j<lines_to_fill)   // ... if it in the first lines ...
                {
                    board[i][j]=PION_NOIR; // ... it is black
                }
                else if (j >= ysize-lines_to_fill)
                {
                    board[i][j] = PION_BLANC;
                }
                else
                {
                    board[i][j] = EMPTY;
                }
            }
            else
            {
                board[i][j]=EMPTY; //surely empty
            }
        }
    }
    nb_white = round(lines_to_fill*xsize/2); // complex formula for the number of pieces
    nb_black = nb_white;
    return board;
}

void print_board(const struct game *game)
{

    //spec : print on the shell the current state of the board
    printf("  ");
    for (int i=0; i<game->xsize; i++)
    {
        printf(" %3d",i);
    }
    printf("\n");
    for (int j=0; j<game->ysize; j++)
    {
        printf("%3d",j);
        for (int i=0; i<game->xsize; i++)
        {
            int current_piece = game->board[i][j];
            if (current_piece == EMPTY)
            {
                printf("|   ");
            }
            else if (is_white(current_piece))
            {
                if (is_dame(current_piece))
                {
                    printf("|");
                    printf("\x1B[31m W ");
                    printf("\033[0m");
                }
                else
                {
                    printf("| w ");
                }
            }
            else
            {
                if (is_dame(current_piece))
                {
                    printf("|");
                    printf("\x1B[31m B ");
                    printf("\033[0m");
                }
                else
                {
                    printf("| b ");
                }
            }
        }
        printf("|\n");
        printf(" ---");
        for(int a=0; a<game->xsize; a++) //because of resizable board ;)
        {
			printf("----");
		}
		printf("\n");
    }
}
