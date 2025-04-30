//
// Created by Yola M on 30/04/2025.
//

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
void generateSudoku(Game *game);


#endif //SUDOKU_H
