#include <stdio.h>
#include "mine_sweeper.c"

int main(void) {
    MineSweeper* ms = constructMineSweeper(4, 4, 4);
    play(ms);
    free(ms);
}
