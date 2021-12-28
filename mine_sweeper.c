#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "colors.h"

typedef struct {
    enum {FLAGGED, REVEALED, CONCEALED} status;
    int cell_value;
} Cell;

typedef struct {
    int rows;
    int cols;
    int mines;
    Cell** grid;
    char** display_grid;
} MineSweeper;

MineSweeper* constructMineSweeper(int rows, int cols, int mines);
void setMines(MineSweeper* ms);
void printGrid(MineSweeper* ms);

MineSweeper* constructMineSweeper(int rows, int cols, int mines) {
    MineSweeper* ms = (MineSweeper*) (malloc(sizeof(MineSweeper)));
    if (ms == NULL) {
        return NULL;
    }
    ms->rows = rows;
    ms->cols = cols;
    ms->mines = mines;
    ms->grid = (Cell**) (malloc(rows * sizeof(Cell*)));
    ms->display_grid = (char**) (malloc(rows * sizeof(char*)));
    if (ms->grid == NULL || ms->display_grid == NULL) {
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        ms->grid[i] = (Cell*) (malloc(cols * sizeof(Cell)));
        ms->display_grid[i] = (char*) (malloc(cols * sizeof(char)));
        if (ms->grid[i] == NULL || ms->display_grid[i] == NULL) {
            return NULL;
        }
        for (int j = 0; j < cols; j++) {
            ms->grid[i][j].cell_value = 0;
            ms->grid[i][j].status = CONCEALED;
            ms->display_grid[i][j] = ' ';
        }
    }
    setMines(ms);
    return ms;
}

void setMines(MineSweeper* ms) {
    int rows, cols, mines, row_index, col_index;
    rows = ms->rows;
    cols = ms->cols;
    mines = ms->mines;
    Cell** grid = ms->grid;
    srand(time(0));
    for (int i = 0; i < mines; i++) {
        do {
            row_index = rand() % rows;
            col_index = rand() % cols;
        } while (grid[row_index][col_index].cell_value == -1);
        grid[row_index][col_index].cell_value = -1;
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