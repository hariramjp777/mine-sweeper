#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "colors.h"

typedef struct {
    int rows;
    int cols;
    int mines;
    int flagged_mines;
    int** grid;
    char** display_grid;
} MineSweeper;

MineSweeper* constructMineSweeper(int rows, int cols, int mines);
void fillGrid(MineSweeper* ms);
void printGrid(MineSweeper* ms);
void setMines(MineSweeper* ms);
int countNeighbours(MineSweeper* ms, int x, int y);

MineSweeper* constructMineSweeper(int rows, int cols, int mines) {
    MineSweeper* ms = (MineSweeper*) (malloc(sizeof(MineSweeper)));
    if (ms == NULL) {
        return NULL;
    }
    ms->rows = rows;
    ms->cols = cols;
    ms->mines = mines;
    ms->flagged_mines = 0;
    ms->grid = (int**) (malloc(rows * sizeof(int*)));
    ms->display_grid = (char**) (malloc(rows * sizeof(char*)));
    if (ms->grid == NULL || ms->display_grid == NULL) {
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        ms->grid[i] = (int*) (malloc(cols * sizeof(int)));
        ms->display_grid[i] = (char*) (malloc(cols * sizeof(char)));
        if (ms->grid[i] == NULL || ms->display_grid[i] == NULL) {
            return NULL;
        }
        for (int j = 0; j < cols; j++) {
            ms->grid[i][j] = 0;
            ms->display_grid[i][j] = ' ';
        }
    }
    fillGrid(ms);
    return ms;
}

void fillGrid(MineSweeper* ms) {
    setMines(ms);
    int rows, cols;
    rows = ms->rows;
    cols = ms->cols;
    int** grid = ms->grid;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == -1) {
                continue;
            }
            grid[i][j] = countNeighbours(ms, i, j);
        }
    }
}

void setMines(MineSweeper* ms) {
    int rows, cols, mines, row_index, col_index;
    rows = ms->rows;
    cols = ms->cols;
    mines = ms->mines;
    int** grid = ms->grid;
    srand(time(0));
    for (int i = 0; i < mines; i++) {
        do {
            row_index = rand() % rows;
            col_index = rand() % cols;
        } while (grid[row_index][col_index] == -1);
        grid[row_index][col_index] = -1;
    }
}

int countNeighbours(MineSweeper* ms, int x, int y) {
    int rows, cols, count, neighbour_x, neighbour_y;
    rows = ms->rows;
    cols = ms->cols;
    int** grid = ms->grid;
    count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            neighbour_x = x + i;
            neighbour_y = y + j;
            if ((neighbour_x >= 0 && neighbour_x < rows) && (neighbour_y >= 0 && neighbour_y < cols)) {
                if (grid[neighbour_x][neighbour_y] == -1) {
                    count++;
                }
            }
        }
    }
    return count;
}

bool leftClick(MineSweeper* ms, int x, int y) {
    int** grid = ms->grid;
    char** display_grid = ms->display_grid;
    if (display_grid[x][y] != ' ') {
        return true;
    }
    if (grid[x][y] == -1) {
        display_grid[x][y] = 'M';
        return false;
    }
    display_grid[x][y] = grid[x][y] + '0';
    return true;
}

void rightClick(MineSweeper* ms, int x, int y) {
    int** grid = ms->grid;
    char** display_grid = ms->display_grid;
    if (display_grid[x][y] == 'F') {
        display_grid[x][y] = ' ';
        ms->flagged_mines--;
        return;
    }
    if (display_grid[x][y] != 'F') {
        display_grid[x][y] = 'F';
        ms->flagged_mines++;
        return;
    }
}

void printGrid(MineSweeper* ms) {
    for (int i = 0, rows = ms->rows, cols = ms->cols; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("[%c] ", ms->display_grid[i][j]);
        }
        printf("\n");
    }
}