#include <stdio.h>
#include <stdlib.h>

struct game *new_game(int xsize, int ysize) {
    ///malloc create space for new game
    game *Newgame = (game *) (calloc(sizeof(game)));
    if (Newgame == NULL) {
        return NULL;
    }
    Newgame->xsize = xsize;
    Newgame->xsize = ysize;
    Newgame->cur_player = PLAYER_WHITE;
    //fill board
    fill_board((*Newgame).board,xsize,ysize);
    return Newgame;
    //return pointer from malloc

}

struct game *load_game(int xsize, int ysize, const int **board, int cur_player) {
    ///Create New game and set position from board and current player
    game *f = new_game(xsize,ysize);
    f->board = board;
    f->cur_player = cur_player;
    return f;
}
void free_game(struct game *game) {
    /// use free() to clear memory
    /// maybe also **board from game and move????
    free(game);
    *game = NULL;

    return;
}

int apply_moves(struct game *game, const struct move *moves) {
    //
    struct move_seq mvseq = (*moves).seq;
    // check if movement is permitted
    while (*mvseq).next != NULL) {
    int ans = is_move_seq_valid(game, mvseq, prev, taken coordinates);
        if (ans == 0) {
            return -1; // error, movement not permitted

        }
        if (ans == 1) {
            // movement permit
            // Set value board[c_old] to board[c_new]
        } else {
            // movement permit et capture
            // Set value board[c_old+1] to board[c_new+1]
        }
        // save move in Move from game
    }
    // when finished, set cur_player to next one
}

int is_move_seq_valid(const struct game *game, const struct move_seq *seq, const struct move_seq *prev, struct coord *taken) {


}

int undo_moves(struct game *game, int n) {
    if(game->moves == NULL) {
        return(EXIT_FAILURE); //si pas d'éléments, nous ne savons rien retirer
    }
    move *iter = (move *) malloc(sizeof(move));
    iter = game->moves; //pas sur de la validité de cette ligne
    int temp;
    for(temp=0; (*iter.next).next != NULL, *iter=iter->next) {
        temp
    }
    iter.next=NULL;
}

void fill_board(int **board, int xsize, int ysize) {
/// Minimal value for Xsize: 4
/// Minimal value for Ysize: 4
/// Maximal number of non-empty lines : 4

    int pion_noir = 0b00000001;
    int pion_blanc = 0b00000101;
    int empty = 0b00000000;
    int lines_to_fill = 0;
    if ((ysize % 2) == 0) {// Table size is even so Nomands land is 2
        int lines_to_fill = (ysize-2)/2;
    } else {
        int lines_to_fill = (ysize-1)/2;
    }
    if(lines_to_fill > 4) { //pas plus de quatre lignes de jeu
        lines_to_fill = 4;
    }
    for (int i=0; i<ysize; i++)
    {
        for (int j=0; j<xsize; j++)
        {
            if(i+j % 2 == 0) { // si case à remplir et ...
                if(j<lines_to_fill) { // ... que dans les 4 premières lignes ...
                    board[i][j]=pion_noir; // ... c'est un pion noir
                }
                if(j>=xsize-lines_to_fill) { // ... que dans les 4dernières lignes ...
                    board[i][j]=pion_blanc; // ... c'est un pion blanc
                }
            }
            else {
                board[i][j]=empty; //sinon vide à coup sure
            }
            //if (j >= lines_to_fill && j < (ysize-lines_to_fill))   // We are in No mans-land
            //{
            //    board[i][j] = empty;
            //}
            //if (i==0)   //first line Black
            //{
            //    board[i][j] = pion_noir;
            //    j++;
            //}
            //else if (7 < *(board[i-1][j]) == 1 && j < lines_to_fill)
            //{
                // pion is juste above, move to next to right
            //    *(board[i][j]) = empty;
            //    j++;
            //}
            //else
            //{
            //    board[i][j] = pion_noir;
            //}
            //entière. Le bit C est le bit de couleur: 0 = noir, 1 = blanc.
            //Le bit T est le bit de type de pièce: 0 = pion, 1 = dame.
            //Le bit P est le bit de présence: 0 = case vide, 1 = case remplie.
        }
    }
}
