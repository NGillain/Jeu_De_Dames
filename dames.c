#include <stdio.h>
#include <stdlib.h>
#include "dames.h"

#define PION_NOIR    0b00000001
#define DAME_NOIRE   0b00000011
#define PION_BLANC   0b00000101
#define DAME_BLANCHE 0b00000111
#define EMPTY        0b00000000

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
    Newgame->cur_player = PLAYER_WHITE; // jouer blanc commence toujours
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
    for (int i=0; i<game->xsize; i++)
    {
        free(game->board[i]);
    }
    free(game->board);
    game->board = NULL;
    // FREE game ! after board
    free(game);
    game=NULL;
    return;
}

int apply_moves(struct game *game, const struct move *moves)
{
    //spec : apply a move by adding it to the chained list and set the current player to the oppenent
    // the move is supposed to be legal
    struct move *iter = (struct move *) malloc(sizeof(struct move *)); //malloc to keep the pointer on the heap
    if (iter==NULL)
    {
        return EXIT_FAILURE; //no more space for pointer
    }
    iter = game->moves; // verifie si la synthaxe est correcte.
    while (iter->next != NULL)
    {
        iter=iter->next;
    }
    iter->next = moves; //add the move
    game->cur_player = !game->cur_player; //other player's turn
    free(iter);

    struct move_seq *iter_seq = (struct move_seq*) malloc(sizeof(struct move_seq *));
    if (iter_seq == NULL) {
        return -1;
    }
    iter_seq = game->moves->seq;
    struct coord taken;
    struct move_seq *prev = NULL;
    while (iter_seq != NULL)
    {
        int valid = is_move_seq_valid(game,iter_seq,prev,&taken);
        int piece = iter->seq->old_orig;
        if (valid == 0 ) {
            return -1; // ERREUR du mouvement ou autre
        } else {
            game->board[iter_seq->c_new.x][iter_seq->c_new.y] = piece; // move old pawn position to new
            game->board[iter_seq->c_old.x][iter_seq->c_old.y] = EMPTY;
            if ((is_white(piece) && iter_seq->c_new.y == 0) || (is_black(piece) && iter_seq->c_new.y == (game->ysize-1))) {
                game->board[iter_seq->c_new.x][iter_seq->c_new.y] = upgrade(game->board[iter_seq->c_new.x][iter_seq->c_new.y]);
            }
            if (valid == 2) {
                //capture
                game->board[iter_seq->piece_taken.x][iter_seq->piece_taken.y] = EMPTY;
            }
        }
        prev = iter_seq;
        iter = iter->next;
    }
    free(iter_seq);
    return 0;
}

int is_dame(int value) // pas sure que cela fonctionne
{
    //spec : 0 si pion et 1 si dame
    return ((value & 0b00000010) && (value & 0b00000001)); // plus simple
}

int is_white(int value) // idem
{
    //spec : 0 si noir et 1 si blanc
    return ((value & 0b00000100) && (value & 0b00000001)); //plus simple
}

int is_move_seq_valid(const struct game *game, const struct move_seq *seq, const struct move_seq *prev, struct coord *taken)
{
    //spec : check ONLY IF one single sequence is valid
    if(game->cur_player!=is_white(game->board[seq->c_old.x][seq->c_old.y]))
    {
        return 0; // if the piece and the player have different colors ==> move is illegal
    }
    if (prev != NULL)  //si prev != NULL check prev->c
    {
        if ((prev->c_new) != (seq->c_old)) //if the new position don't begin at the end position of the previous move ==> move is illegal
        {
            return 0;
        }
    }
    int xold = seq->c_old.x;
    int yold = seq->c_old.y;
    int xnew = seq->c_new.x;
    int ynew = seq->c_new.y
    int c = is_white(game->board[xold][yold]);
    if (game->board[xold][yold] == EMPTY || game->board[xnew][ynew] != EMPTY || abs(xold-xnew) != abs(yold-ynew))
    {
        return 0; // case ou on veut aller non vide ou un mouvement de déplacement non diagonal ou encore une case vide
    }
    if (0>xnew || xnew>game->xsize || 0>ynew || ynew>game->ysize)
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
        if(seq->next == NULL && temp==1 && game->board[xnew+(xold-xnew)/(abs(xold-xnew))][ynew+(yold-ynew)/(abs(yold-ynew))] == EMPTY))
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
            if (abs(xold-xnew) == 2 && !is_white(game->board[xold+(xnew-xold)/2][yold+(ynew-yold)/2] && game->board[xold+(xnew-xold)/2][yold+(ynew-yold)/2] != EMPTY) // check si il y a un pion sur la casse ou le pion est passé au dessus
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
    //spec : get n moves backwards by deleting n nodes
    for(int a=0, a<n, a++){
        if(game->moves == NULL)
        {
            return(EXIT_FAILURE); //si pas d'éléments, nous ne savons rien retirer
        }
        struct move *iter = (struct move *) (malloc(sizeof(struct move)));
        iter = game->moves; //pas sur de la validité de cette ligne
        if(iter == NULL)
        {
            return(EXIT_FAILURE); //pas d'espace
        }
        while(*(iter->next).next != NULL)
        {
            iter=iter->next;
        }
        iter.next=NULL;
        free(iter);
    }
    if(n%2==1){
       game->cur_player = !game->cur_player; //changement de joueur si nombre de coups annulés impairs
    }
}


void create_board(int **board, int xsize, int ysize)
{
/// Minimal value for Xsize: 5
/// Minimal value for Ysize: 5

    //spec : initialize a game board with length x and height y
    int lines_to_fill = 0;
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
                else if (j>=xsize-lines_to_fill)     // ... que dans les 4 dernières lignes ...
                {
                    board[i][j]=PION_BLANC; // ... c'est un pion blanc
                }
                else
                {
                    board[i][j]=EMPTY; //sinon c'est vide (?)
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
            else if (is_white(current_piece))
            {
                if (is_dame(current_piece))
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
                if (is_dame(current_piece))
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
