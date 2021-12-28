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
    fillGrid(ms);
    return ms;
}

void fillGrid(MineSweeper* ms) {
    setMines(ms);
    int rows, cols;
    rows = ms->rows;
    cols = ms->cols;
    Cell** grid = ms->grid;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j].cell_value == -1) {
                continue;
            }
            grid[i][j].cell_value = countNeighbours(ms, i, j);
        }
    }
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

int countNeighbours(MineSweeper* ms, int x, int y) {
    int rows, cols, count, neighbour_x, neighbour_y;
    rows = ms->rows;
    cols = ms->cols;
    Cell** grid = ms->grid;
    count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            neighbour_x = x + i;
            neighbour_y = y + j;
            if ((neighbour_x >= 0 && neighbour_x < rows) && (neighbour_y >= 0 && neighbour_y < cols)) {
                if (grid[neighbour_x][neighbour_y].cell_value == -1) {
                    count++;
                }
            }
        }
    }
    return count;
}

void printGrid(MineSweeper* ms) {
    for (int i = 0, rows = ms->rows, cols = ms->cols; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("[%c] ", ms->display_grid[i][j]);
        }
        printf("\n");
    }
}