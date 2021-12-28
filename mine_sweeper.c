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
void fillGrid(MineSweeper* ms, int start_x, int start_y);
void setMines(MineSweeper* ms, int start_x, int start_y);
void printGrid(MineSweeper* ms);
int countNeighbours(MineSweeper* ms, int x, int y);
int leftClick(MineSweeper* ms, int x, int y);
void rightClick(MineSweeper* ms, int x, int y);
void openSurroundings(MineSweeper* ms, int x, int y);
void gameOver(MineSweeper* ms);
bool goalAchieved(MineSweeper* ms);
void play(MineSweeper* ms);
bool checkPos(MineSweeper* ms, int pos_x, int pos_y);
bool areNeighbours(int row_index, int col_index, int start_x, int start_y);

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
    return ms;
}

void fillGrid(MineSweeper* ms, int start_x, int start_y) {
    setMines(ms, start_x, start_y);
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

void setMines(MineSweeper* ms, int start_x, int start_y) {
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
        } while (areNeighbours(row_index, col_index, start_x, start_y) || grid[row_index][col_index] == -1);
        grid[row_index][col_index] = -1;
    }
}

bool areNeighbours(int row_index, int col_index, int start_x, int start_y) {
    int neighbour_x, neighbour_y;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            neighbour_x = start_x + i;
            neighbour_y = start_y + j;
            if (neighbour_x == row_index && neighbour_y == col_index) {
                return true;
            }
        }
    }
    return false;
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

int leftClick(MineSweeper* ms, int x, int y) {
    int** grid = ms->grid;
    char** display_grid = ms->display_grid;
    if (display_grid[x][y] != ' ') {
        return 0;
    }
    static bool first_click = true;
    if (first_click) {
        fillGrid(ms, x, y);
        first_click = false;
    }
    if (grid[x][y] == -1) {
        display_grid[x][y] = 'M';
        return -1;
    }
    if (grid[x][y] == 0) {
        openSurroundings(ms, x, y);
    }
    display_grid[x][y] = grid[x][y] + '0';
    if (goalAchieved(ms)) {
        return 1;
    }
    return 0;
}

void openSurroundings(MineSweeper* ms, int x, int y) {
    int rows, cols, neighbour_x, neighbour_y;
    rows = ms->rows;
    cols = ms->cols;
    int** grid = ms->grid;
    char** display_grid = ms->display_grid;
    if (display_grid[x][y] != ' ') {
        return;
    }
    if (grid[x][y] != 0) {
        display_grid[x][y] = grid[x][y] + '0';
        return;
    }
    display_grid[x][y] = grid[x][y] + '0';
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            neighbour_x = x + i;
            neighbour_y = y + j;
            if (checkPos(ms, neighbour_x, neighbour_y)) {
                openSurroundings(ms, neighbour_x, neighbour_y);
            }
        }
    }
}

void rightClick(MineSweeper* ms, int x, int y) {
    int** grid = ms->grid;
    char** display_grid = ms->display_grid;
    if (display_grid[x][y] == 'F') {
        display_grid[x][y] = ' ';
        ms->flagged_mines--;
        return;
    }
    if (display_grid[x][y] == ' ' && display_grid[x][y] != 'F') {
        display_grid[x][y] = 'F';
        ms->flagged_mines++;
        return;
    }
}

void gameOver(MineSweeper* ms) {
    int rows, cols, mines, flagged_mines;
    rows = ms->rows;
    cols = ms->cols;
    mines = ms->mines;
    flagged_mines = ms->flagged_mines;
    srand(time(0));
    int row_index, col_index;
    system("cls");
    printf("\n");
    printGrid(ms);
    sleep(1.7);
    for (int i = 0; i < mines - 1; i++) {
        do {
            row_index = rand() % rows;
            col_index = rand() % cols;
        } while (!(ms->grid[row_index][col_index] == -1 && ms->display_grid[row_index][col_index] != 'M'));
        if (ms->display_grid[row_index][col_index] == 'F') {
            ms->grid[row_index][col_index] = (ms->grid[row_index][col_index] == -1) ? -1 : -2;
            continue;
        }
        ms->display_grid[row_index][col_index] = 'M';
        system("cls");
        printf("\n");
        printGrid(ms);
        sleep(1.7);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (ms->grid[i][j] == -2 || (ms->display_grid[i][j] == 'F' && ms->grid[i][j] != -1)) {
                ms->display_grid[i][j] = 'X';
            }
        }
    }
}

bool goalAchieved(MineSweeper* ms) {
    if (ms->mines != ms->flagged_mines) {
        return false;
    }
    for (int i = 0, rows = ms->rows, cols = ms->cols; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (ms->display_grid[i][j] == ' ' || (ms->display_grid[i][j] == 'F' && ms->grid[i][j] != -1)) {
                return false;
            }
        }
    }
    return true;
}

void printGrid(MineSweeper* ms) {
    for (int i = 0, rows = ms->rows, cols = ms->cols; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char ch = ms->display_grid[i][j];
            printf(F_YELLOW "[" RESET);
            if (ch == 'M') {
                printf(REDHB "%c" RESET, '*');
            }
            else if (ch == 'F') {
                printf(F_BLUE "%c" RESET, ch);
            }
            else if (ch == 'X') {
                printf(F_RED "%c" RESET, ch);
            }
            else {
                printf(F_B_PURPLE "%c" RESET, ch);
            }
            printf(F_YELLOW "] " RESET);
        }
        printf("\n");
    }
}

void play(MineSweeper* ms) {
    int option, pos_x, pos_y;
    int prev = 1;
    system("cls");
    while (true) {
        if (prev == 0) {
            printf(F_RED "<Invalid Option>\n" RESET);        
        }
        printf("\n");
        printf(F_YELLOW "Flags: %d\n" RESET, (ms->mines - ms->flagged_mines));
        printf("\n");
        printGrid(ms);
        printf("\n");
        printf(F_YELLOW "> " RESET F_D_BLUE "'1' " RESET "for LeftClick\n");
        printf(F_YELLOW "> " RESET F_D_BLUE "'2' " RESET "for RightClick\n");
        printf(F_YELLOW "> " RESET F_D_BLUE "'3' " RESET "for Quit\n");
        printf("\n" F_YELLOW ">> " RESET);
        scanf("%d", &option);
        if (option < 0 || option > 3) {
            prev = 0;
            system("cls");
            continue;
        }
        prev = 1;
        if (option == 3) {
            return;
        }
        do {
            printf("\n" F_YELLOW ">> x: " RESET);
            scanf("%d", &pos_x);
            printf(F_YELLOW ">> y: " RESET);
            scanf("%d", &pos_y);
        } while (checkPos(ms, pos_x, pos_y) == false);
        if (option == 2) {
            rightClick(ms, pos_x, pos_y);
            system("cls");
            continue;
        }
        if (option == 1) {
            int result = leftClick(ms, pos_x, pos_y);
            if (result == -1) {
                gameOver(ms);
                system("cls");
                printf("\n");
                printGrid(ms);
                printf("\n");
                printf(F_RED "Game Over\n" RESET);
                break;
            }
            if (result == 1) {
                goalAchieved(ms);
                system("cls");
                printf("\n");
                printGrid(ms);
                printf("\n");
                printf(F_GREEN "You win the game\n" RESET);
                break;
            }
            system("cls");
        }
    }
}

bool checkPos(MineSweeper* ms, int pos_x, int pos_y) {
    return (pos_x >= 0 && pos_x < ms->rows) && (pos_y >= 0 && pos_y < ms->cols);
}