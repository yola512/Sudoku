#ifndef SUDOKU_H
#define SUDOKU_H
#include "utils.h"

bool unUsedInRow(int **board, int i, int number, int boardSize);
bool unUsedInColumn(int **board, int j, int number, int boardSize);
bool unUsedInBox(int **board, int rowStart, int colStart, int number, int boardSize);
bool finalCheck(int **board, int i, int j, int number, int boardSize);
void fillBox(int **board, int row, int col, int boardSize);
void fillDiagonal(int **board, int boardSize);
bool fillRemaining(int **board, int i, int j, int boardSize);
void removeXDigits(int **board, int x, int boardSize);
int getNumberOfCellsToRemove(int boardSize, Difficulty difficulty);
int **generateSudoku(Difficulty difficulty, int boardSize, Game *game);


#endif //SUDOKU_H
