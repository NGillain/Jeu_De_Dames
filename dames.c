#include <stdio.h>
#include <stdlib.h>

#define PION_NOIR = 0b00000001;
#define PION_BLANC = 0b00000101;
#define EMPTY = 0b00000000;

struct game *new_game(int xsize, int ysize)
{
    ///malloc create space for new game
    game *Newgame = (game *) (malloc(sizeof(game)));
    if (Newgame == NULL)
    {
        return NULL;
    }
    (*Newgame).xsize = xsize;
    Newgame->xsize = xsize;
    (*Newgame).ysize = ysize;
    (*Newgame).cur_player = PLAYER_WHITE;
    //fill board
    fill_board((*Newgame).board,xsize,ysize);
    return Newgame;
    //return pointer from malloc

}

struct game *load_game(int xsize, int ysize, const int **board, int cur_player)
{
    ///Create New game and set position from board and current player
    game *f = new_game(xsize,ysize);
    (*f).board = board;
    (*f).cur_player = cur_player;
    return f;
}
void free_game(struct game *game)
{
    free(game);
    *game = NULL;

    return;
}

int apply_moves(struct game *game, const struct move *moves)
{
    //
    struct move_seq mvseq = (*moves).seq;
    // check if movement is permitted
    while (mvseq->next != NULL)
    {
        int ans = is_move_seq_valid(game, mvseq, prev, taken coordinates);
        if (ans == 0)
        {
            return -1; // error, movement not permitted

        }
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

int is_dame(const board **board,int x,int y)
{
    return (((board[x][y]) >> 1 ) << 7) == 0b1;
}

int is_blanc(const board **board,int x,int y)
{
    return ((board[x][y]) >> 2) == 0b1;
}

int is_move_seq_valid(const struct game *game, const struct move_seq *seq, const struct move_seq *prev, struct coord *taken)
{
    while (tmp>next != NULL) //?????????????????????????,,,
    {
        int xold = seq->c_old->x;
        int yold = seq->c_old->y;
        int xnew = seq->c_new->x;
        int ynew = seq->c_new->y;
        if (game->board[xnew][ynew] != EMPTY || abs(xold-xnew) != abs(xold-xnew))
        {
            return 0; // casse ou on veut aller non vide ou un mouvement de déplacement non valide
        }
        if (is_dame(game->board,xold,yold))   // on à une dame qui bouge
        {
            if (seq->next == NULL && game->board[xnew+(xold-xnew)/(abs(xold-xnew))][ynew+(yold-ynew)/(abs(yold-ynew))] == EMPTY)
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
            if (abs(xold-xnew) > 2)
            {
                return 0;
            }
            if (is_blanc(game->board,xold,yold))
            {
                if (abs(xold-xnew) = 2 && is_blanc(game->board,xold+(xnew-xold)/2,yold+(ynew-yold)/2)))
                {
                    return 0;
                }
                else
                {
                    return 2; // capture d'un noir par les blanc
                }
                if ((xold-xnew) < 0)
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
                if ((xold-xnew) > 0)
                {
                    return 0;
                }
            }
        }

    }

    return 1; // OK, deplacement
}

int undo_moves(struct game *game, int n)
{
    if(game->moves == NULL)
    {
        return(EXIT_FAILURE); //si pas d'éléments, nous ne savons rien retirer
    }
    move *iter = (move *) malloc(sizeof(move));
    iter = game->moves; //pas sur de la validité de cette ligne
    while(*(iter->next).next != NULL)
    {
        iter=iter->next;
    }
    iter.next=NULL;
    free(iter);
}


void fill_board(int **board, int xsize, int ysize)
{
/// Minimal value for Xsize: 5
/// Minimal value for Ysize: 5

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
            //board[i][j] = (int *)(malloc(sizeof(int))); // malloc for
            /// i+j % 2 == 0 => pion to place

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
    // print line by line
}
