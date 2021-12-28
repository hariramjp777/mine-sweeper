#include <stdio.h>
#include "mine_sweeper.c"

int main(void) {
    int rows = 8;
    int cols = 10;
    int mines = 10;
    MineSweeper* ms = constructMineSweeper(rows, cols, mines);
    play(ms);
    free(ms);
}
