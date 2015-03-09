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
}
