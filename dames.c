#include <stdio.h>
#include <stdlib.h>
#include <math.h> //compilé avec -lm
#include "dames.h"

#define PION_NOIR   0b00000001
#define PION_BLANC  0b00000101
#define EMPTY       0b00000000

int nb_white = 0;
int nb_black = 0;
int moves_done = 0;


struct game *new_game(int xsize, int ysize)
{
    //spec : create a new struct representing a game
    struct game *Newgame = (struct game *) (malloc(sizeof(struct game)));
    if (Newgame == NULL)
    {
        return NULL;
    }
    Newgame->xsize = xsize;
    Newgame->ysize = ysize;
    nb_black = 10; // adapt to board size
    nb_white = 10;
    Newgame->cur_player = PLAYER_WHITE; // jouer blanc commance toujours
    Newgame->moves = NULL;
    Newgame->board = create_board(xsize,ysize);
    if (Newgame->board == NULL)
    {
        return NULL;
    }
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
    //spec : apply a move by adding it to the chained list and set the current player to the oppenent
    struct move *iter_move = moves;
    while (iter_move != NULL)
    {
        struct move_seq *iter_seq;
        iter_seq = iter_move->seq;
        struct coord taken;
        struct move_seq *prev = NULL;
        while (iter_seq != NULL)
        {
            int valid = is_move_seq_valid(game,iter_seq,prev,&taken);
            int piece = game->board[iter_seq->c_old.x][iter_seq->c_old.y];
            if (valid == 0 )
            {
                return -1; // ERREUR du mouvement ou autre
            }
            else
            {
                game->board[iter_seq->c_new.x][iter_seq->c_new.y] = piece; // move old pawn position to new
                game->board[iter_seq->c_old.x][iter_seq->c_old.y] = EMPTY;
                if ((is_white(piece) && iter_seq->c_new.y == 0) || (is_black(piece) && iter_seq->c_new.y == (game->ysize-1)))
                {
                    game->board[iter_seq->c_new.x][iter_seq->c_new.y] = upgrade(game->board[iter_seq->c_new.x][iter_seq->c_new.y]);
                }
                if (valid == 2)
                {
                    //capture
                    game->board[iter_seq->piece_taken.x][iter_seq->piece_taken.y] = EMPTY;
                }
                prev = iter_seq;
                iter_seq = iter_seq->next;
                return 0;
            }
        }
        game->cur_player = !game->cur_player; //other player's turn
        if (nb_black == 0)
        {
            game->cur_player = PLAYER_WHITE;
            return 1; // blanc gagne
        }
        if (nb_white == 0)
        {
            game->cur_player = PLAYER_BLACK;
            return 1; // noir gagne
        }
    }
    // attention si capture final dans seq, cette historique ne sera pas enregistré car returnn avant !!!!!!!!!!!!!!!!!!!!!!!!
    struct move *iter_move_game = game->moves;
    struct move *iter_move_game_prev = NULL;
    while (iter_move_game != NULL)
    {
        iter_move_game_prev = iter_move_game;
        iter_move_game = iter_move_game->next;
    }
    iter_move_game_prev->next = moves;
    return 0;
}

void reverse(struct move_seq *head_ref)
{
    struct move_seq *prev = NULL;
    struct move_seq *current = head_ref;
    struct move_seq *next;
    while (current != NULL)
    {
        next  = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head_ref = prev;
}

int upgrade(int value)
{
    return (value | 0b00000010);
}

int is_dame(int value) // pas sure que cela fonctionne
{
    //spec : 0 si pion et 1 si dame
    return (((value >> 1 ) << 7) == 0b10000000);
}

int is_white(int value) // idem
{
    //spec : 0 si noir et 1 si blanc
    return ((value >> 2) == 0b00000001);
}

int is_black(int value)
{
    return ((value >> 2) == 0b00000000);
}

int is_move_seq_valid(const struct game *game, const struct move_seq *seq, const struct move_seq *prev, struct coord *taken)
{
    //spec : check ONLY IF one single sequence is valid
    if ((game->cur_player ) != is_white(game->board[seq->c_old.x][seq->c_old.y]))
    {
        return 0; // if the piece and the player have different colors ==> move is illegal
    }
    if (prev != NULL)  //si prev != NULL check prev->c
    {
        if ((prev->c_new.x) != (seq->c_old.x)) //if the new position don't begin at the end position of the previous move ==> move is illegal
        {
            return 0;
        }
    }
    int xold = seq->c_old.x;
    int yold = seq->c_old.y;
    int xnew = seq->c_new.x;
    int ynew = seq->c_new.y;
    int c = is_white(game->board[xold][yold]);
    if (game->board[xold][yold] == EMPTY || game->board[xnew][ynew] != EMPTY || abs(xold-xnew) != abs(yold-ynew))
    {
        return 0; // case ou on veut aller non vide ou un mouvement de déplacement non diagonal ou encore une case vide
    }
    if (0>xnew || xnew>(game->xsize) || 0>ynew || ynew>(game->ysize))
    {
        return 0; // illegal move : OutOfBound
    }
    if (is_dame(game->board[xold][yold]))   // on à une dame qui bouge
    {
        int vec_X = (xnew-xold)/abs(xnew-xold); //vecteur unitaire du déplacement du pion/dame en x
        int vec_Y = (ynew-yold)/abs(ynew-yold); //vecteur unitaire du déplacement du pion/dame en y
        for(int a=1; a<=abs(xnew-xold); a++)
        {
            if(c == is_white(game->board[xold+a*vec_X][yold+a*vec_Y])) //Des qu'il y a un pion de la même couleur
            {
                return 0;
            }
        }
        int temp=0;
        for(int b=1; b<=(xnew-xold); b++)
        {
            if(c != is_white(game->board[xold+b*vec_X][yold+b*vec_Y]))
            {
                temp++; // on a trouvé un pion de la couleur opposée
            }
            if(temp>1) // Si plus d'un pion de couleur opposée, illegal move
            {
                return 0;
            }
        }
        if (seq->next == NULL && temp==1 && game->board[xnew+(xold-xnew)/(abs(xold-xnew))][ynew+(yold-ynew)/(abs(yold-ynew))] == EMPTY)
        {
            return 0; // si la dame finit son move et qu'elle se positionne qqpart où il n'y pas de pions avant
        }
        else if(temp==0)
        {
            return 1; // si rien le mvt est valide
        }
        else
        {
            return 2; //sinon c'est que le mvt est valide et la dame prend un pion
        }
    }
    else   // on a un pion qui bouge
    {
        if (abs(xold-xnew) > 2) // pion ne bouge pas de plus de 2 casses
        {
            return 0;
        }
        if (c) // si la piece est blanche
        {
            if (abs(xold-xnew) == 2 && !is_white(game->board[xold+(xnew-xold)/2][yold+(ynew-yold)/2]) && game->board[xold+(xnew-xold)/2][yold+(ynew-yold)/2] != EMPTY) // check si il y a un pion sur la casse ou le pion est passé au dessus
            {
                return 2;// capture d'un noir par les blanc
            }
            if ((yold-ynew) < 0) // les blanc bouge vers le haut, vers y=0
            {
                return 0;
            }
        }
        else //si elle est noire
        {
            if (abs(xold-xnew) == 2 && is_white(game->board[xold+(xnew-xold)/2][yold+(ynew-yold)/2]))
            {
                return 2;
            }
            if ((yold-ynew) > 0) // les noirs bouge vers le bas, vers y=ysize
            {
                return 0;
            }
        }
    }

    return 1; // OK, deplacement possible sans capture
}

int undo_moves(struct game *game, int n)
{
    reverse(game->moves);
    struct move *iter_move = game->moves;
    int undo_done = 0;
    while (iter_move != NULL && undo_done < n)
    {
        reverse(iter_move->seq);
        struct move_seq *iter_seq = iter_move->seq;
        while(iter_seq != NULL)   // reset new coordiantes to old and set pawn back if where taken
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
    reverse(game->moves);
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
    while ( (iter_move != NULL) && (removed < n))
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
    if(lines_to_fill>4)
    {
        lines_to_fill = 4; // max 4 lines with pions
    }
    for (int i=0; i<xsize; i++)
    {
        board[i] = (int *) malloc(ysize*sizeof(int));
        for (int j=0; j<ysize; j++)
        {

            if ( ((i+j)%2) == 0)   // si case à remplir et ...
            {
                if (j<lines_to_fill)   // ... que dans les 4 premières lignes ...
                {
                    board[i][j]=PION_NOIR; // ... c'est un pion noir
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
                board[i][j]=EMPTY; //sinon vide à coup sure
            }
        }
    }
    return board;
}

void print_board(const struct game *game)
{

    //spec : print on the shell the board
    for (int i=0; i<game->xsize; i++)
    {
        printf(" %3d",i);
    }
    printf("\n");
    for (int j=0; j<game->ysize; j++)
    {
        printf("%d",j);
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
        printf("--------------------------------------------\n");
    }
}
