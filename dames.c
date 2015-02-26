#include <stdio.h>
#include <stdlib.h>

struct game *new_game(int xsize, int ysize) {
    ///malloc create space for new game
    game *Newgame = (game *) (malloc(sizeof(game)));
    if (Newgame == NULL) {
        return NULL;
    }
    (*Newgame).xsize = xsize;
    (*Newgame).xsize = ysize;
    return Newgame;
    //return pointer from malloc

}

struct game *load_game(int xsize, int ysize, const int **board, int cur_player) {
    ///Create New game and set position from board and current player
    game *f = new_game(xsize,ysize);
    (*f).board = board;
    (*f).cur_player = cur_player;
    return f;
}
void free_game(struct game *game) {
    /// use free() to clear memory
    // maybe also **board from game and move????
    free(game);
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

}

