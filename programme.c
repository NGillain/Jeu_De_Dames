#include <stdio.h>
#include <stdlib.h>
#include "dames.c"


int main(int argc, int *argv[]){
    struct game *Jeu = new_game(10,10);//set up game
    print_board(Jeu); // show game
    struct coord old = {1,3};
    struct coord newC = {2,4};
    struct move_seq f ={NULL,old,newC,0,NULL,0};
    struct move mv = {NULL,&f};
    struct coord ta;
    int r =  is_move_seq_valid(Jeu,&f,NULL,&ta);
    printf("%d",r);
    free_game(Jeu); // stop and free memory from game
    
    // check validity of move_seqs in moves
    struct move_seq *iter_seq = (struct move_seq*) malloc(sizeof(struct move_seq *));
    iter_seq = game->moves->seq;
    struct move_seq *prev = (struct move_seq) calloc(sizeof(move_seq));
    struct coord taken;
    if (taken == NULL) {
        return -1;
    }
    while (iter_seq != NULL)
    {
        int valid = is_move_seq_valid(game,iter_seq,prev,taken);
        if (valid == 0 ) {
            return -1; // ERREUR du mouvement ou autre
        } else if (valid == 2) {
                //capture
                game->board[iter_seq->c_new.x][iter_seq->c_new.y] = game->board[iter_seq->c_old.x][iter_seq->c_old.y]; // move old pawn position to new
                game->board[iter_seq->c_old.x][iter_seq->c_old.y] = EMPTY;
                game->board[taken.x][taken.y] = EMPTY; // remove taken pawn



        } else {
            // simple deplacement
            game->board[iter_seq->c_new.x][iter_seq->c_new.y] = game->board[iter_seq->c_old.x][iter_seq->c_old.y]; // move old pawn position to new
                game->board[iter_seq->c_old.x][iter_seq->c_old.y] = EMPTY;
            return 1;
        }
        prev = iter_seq;
        iter=iter->next;
    }
}
