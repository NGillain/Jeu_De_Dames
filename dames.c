#include <stdio.h>
#include <stdlib.h>
#include "dames.h"

#define PION_NOIR   0b00000001
#define PION_BLANC  0b00000101
#define EMPTY       0b00000000

int main(int argc,char *argv[])
{
    return 1;
}

struct game *new_game(int xsize, int ysize)
{
    //spec : create a new struct representing a game
    struct game *Newgame;
    Newgame = (struct game *) (malloc(sizeof(struct game)));
    if (Newgame == NULL)
    {
        return NULL;
    }
    Newgame->xsize = xsize;
    Newgame->ysize = ysize;
    Newgame->cur_player = PLAYER_WHITE; // jouer blanc commance toujours
    Newgame->moves = NULL; // Null car pas de move au debut
    int **board = (int **) malloc(ysize*sizeof(int*));
    //fill board
    create_board(Newgame->board,xsize,ysize);
    return Newgame;
}

struct game *load_game(int xsize, int ysize, const int **board, int cur_player)
{
    //spec : load a game by initialization
    struct game *f = new_game(xsize,ysize);
    f->board = board;
    f->cur_player = cur_player;
    return f;
}
void free_game(struct game *game)
{
    //spec : free the memory of useless games
    free(game);
    game = NULL;
    return;
}

int apply_moves(struct game *game, const struct move *moves)
{
    //spec : apply a move by adding it to the chained list
    // the move is supposed to be legal
    struct move_seq *mvseq = moves->seq;
    while (mvseq->next != NULL)
    {
        //int ans = is_move_seq_valid(game, mvseq, prev, taken coordinates);
        int ans = 0;
        if (ans == 0)
        {
            return -1; // error, movement not permitted

        }
        // gestion of illegal move is done anywhere else
        if (ans == 1)
        {
            // movement permit
            // Set value board[c_old] to board[c_new]
        }
        else
        {
            // movement permit et capture
            // Set value board[c_old+1] to board[c_new+1]
        }
        // save move in Move from game
    }
    // when finished, set cur_player to next one
}

int is_dame(const int **board,int x,int y)
{
    //spec : 0 si pion et 1 si dame
    return (((board[x][y]) >> 1 ) << 7) == 0b1;
}

int is_blanc(const int **board,int x,int y)
{
    //spec : 0 si noir et 1 si blanc
    return ((board[x][y]) >> 2) == 0b1;
}

int is_move_seq_valid(const struct game *game, const struct move_seq *seq, const struct move_seq *prev, struct coord *taken)
{
    //spec : check ONLY IF one single sequence is valid
    if (prev != NULL)   //si prev != NULL check prev->c_
    {
        if ((prev->c_new) != (seq->c_old))
        {
            return 0;
        }
    }
    int xold = seq->c_old->x;
    int yold = seq->c_old->y;
    int xnew = seq->c_new->x;
    int ynew = seq->c_new->y;
    if (game->board[xnew][ynew] != EMPTY || abs(xold-xnew) != abs(yold-ynew))
    {
        return 0; // case ou on veut aller non vide ou un mouvement de déplacement non valide
    }
    if (is_dame(game->board,xold,yold))   // on à une dame qui bouge
    {
        if (seq->next == NULL && game->board[xnew+(xold-xnew)/(abs(xold-xnew))][ynew+(yold-ynew)/(abs(yold-ynew))] == EMPTY)//
        {
            return 0;
        }
        else
        {
            return 2; // capture du dernier pion par la dame
        }
    }
    else   // on a un pion qui bouge
    {
        if (abs(xold-xnew) > 2) // pion ne bouge pas de plus de 2 casses
        {
            return 0;
        }
        if (is_blanc(game->board,xold,yold))
        {
            if (abs(xold-xnew) = 2 && is_blanc(game->board,xold+(xnew-xold)/2,yold+(ynew-yold)/2)) // check si il y a un pion sur la casse ou le pion est passé au dessus
            {
                return 0;
            }
            else
            {
                return 2; // capture d'un noir par les blanc
            }
            if ((yold-ynew) < 0) // les blanc bouge vers le haut, vers y=0
            {
                return 0;
            }
        }
        else
        {
            if (abs(xold-xnew) = 2 && !is_blanc(game->board,xold+(xnew-xold)/2,yold+(ynew-yold)/2))
            {
                return 0;
            }
            else
            {
                return 2; // capture d'un blanc par les noir
            }
            if ((yold-ynew) > 0) // les noirs bouge vers le bas, vers y=ysize
            {
                return 0;
            }
        }
    }

    return 1; // OK, deplacement possible
}
int undo_moves(struct game *game, int n)
{
    //spec : get n moves backwards by deleting n nodes
    if(game->moves == NULL)
    {
        return(EXIT_FAILURE); //si pas d'éléments, nous ne savons rien retirer
    }
    struct move *iter = (struct move *) (malloc(sizeof(struct move)));
    iter = game->moves; //pas sur de la validité de cette ligne
    while(*(iter->next).next != NULL)
    {
        iter=iter->next;
    }
    iter.next=NULL;
    free(iter);
}


void create_board(int **board, int xsize, int ysize)
{
/// Minimal value for Xsize: 5
/// Minimal value for Ysize: 5

    //spec : initialize a game board with length x and height y
    int lines_to_fill = 0;
    if ((ysize % 2) == 0)
    {
        int lines_to_fill = (ysize-2)/2;
    }
    else
    {
        int lines_to_fill = (ysize-1)/2;
    }
    if(lines_to_fill>4)
    {
        lines_to_fill = 4;
    }
    for (int i=0; i<ysize; i++)
    {
        for (int j=0; j<xsize; j++)
        {
            board[i][j] = (int *) malloc(xsize*sizeof(int));

            if(i+j % 2 == 0)   // si case à remplir et ...
            {
                if(j<lines_to_fill)   // ... que dans les 4 premières lignes ...
                {
                    board[i][j]=PION_NOIR; // ... c'est un pion noir
                }
                else if (j>=xsize-lines_to_fill)     // ... que dans les 4dernières lignes ...
                {
                    board[i][j]=PION_BLANC; // ... c'est un pion blanc
                }
                else
                {
                    board[i][j]=EMPTY;
                }
            }
            else
            {
                board[i][j]=EMPTY; //sinon vide à coup sure
            }
        }
    }
}

void print_board(const struct game *game)
{

    //spec : print on the shell the board
    for (int j=0; j<game->ysize; j++)
    {
        for (int i=0; i<game->xsize; i++)
        {
            int current_piece = game->board[i][j];
            if (current_piece==EMPTY)
            {
                printf("|   ");
            }
            else if (is_blanc(current_piece,i,j))
            {
                if (is_dame(current_piece,i,j))
                {
                    printf("| W ");
                }
                else
                {
                    printf("| w ");
                }
            }
            else
            {
                if (is_dame(current_piece,i,j))
                {
                    printf("| B ");
                }
                else
                {
                    printf("| b ");
                }
            }
        }
        printf("|\n");
    }
}
