//
// Created by Yola M on 30/04/2025.
//

#include "sudoku.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DIGIT VALIDATION

// check if provided number is safe to put in row i
bool unUsedInRow(int **board, int i, int number, int boardSize) {
    for (int j = 0; j < boardSize; j++) {
        if (board[i][j] == number) {
            return false;
        }
    }
    return true;
}

// check if provided number is safe to put in column j
bool unUsedInColumn(int **board, int j, int number, int boardSize) {
    for (int i = 0; i < boardSize; i++) {
        if (board[i][j] == number) {
            return false;
        }
    }
    return true;
}

// check if 2x2/3x3/4x4 box contains number -> if it does, return false (meaning the number CANNOT be put in this box)
bool unUsedInBox(int **board, int rowStart, int colStart, int number, int boardSize) {
    for (int i = 0; i < sqrt(boardSize); i++) {
        for (int j = 0; j < sqrt(boardSize); j++) {
            if( board[rowStart+i][colStart+j] == number) {
                return false; // number already exists in the box
            }
        }
    }
    return true; // the number can be put in the box
}

// final check if the number isn't used in row, column or box
// (meaning if it's safe to put the number in choosen cell)
bool finalCheck(int **board, int i, int j, int number, int boardSize) {
    return (unUsedInRow(board, i, number, boardSize) && unUsedInColumn(board, j, number, boardSize)
            && unUsedInBox(board, i, j, number, boardSize));
}


// GENERATING SUDOKU + FILLING THE BOARD
// filling 2x2/3x3/4x4 matrix
void fillBox(int **board, int row, int col, int boardSize) {
    int number;
    int r = rand();
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            do {
                number = rand() % boardSize + 1; // between 1-4/1-9/1-16
            }
            while (!unUsedInBox(board, row, col, number, boardSize));

            board[row + i][col + j] = number;
        }
    }
}

// fill diagonal (upper left, middle, lower right) 2x2/3x3/4x4 boxes
void fillDiagonal(int **board, int boardSize) {
  for (int i = 0; i < boardSize; i = i+sqrt(boardSize)) {
    fillBox(board, i, i, boardSize); // row = col in diagonal
  }
}

bool fillRemaining(int **board, int i, int j, int boardSize) {
  // return true if you reach the end of the grid
  if (i >= boardSize) {
    return true;
  }

  // move to the next row when you finish filling the row
  if (j >= boardSize) {
    return fillRemaining(board, i+1, 0, boardSize);
  }

  // skip cell that is already filled
  if (board[i][j] != 0) {
    return fillRemaining(board, i, j+1, boardSize);
  }

  // try inserting numbers 1-4/1-9/1-16
  for (int number = 1; number <= boardSize; number++) {
    // check if the number can be put in the cell
    if (finalCheck(board, i, j, number, boardSize)) {
      board[i][j] = number;

      // recursion used for filling next cells of the board
      if (fillRemaining(board, i, j+1, boardSize)) {
        // if the whole board has been filled properly - stop
        return true;
      }
      // if placing the number doesn't lead to a valid solution
      // remove it from the cell and try another number
      board[i][j] = 0;
    }
  }
  return false;
}

// remove x random digits from the board (set them to 0)
void removeXDigits(int **board, int x, int boardSize) {
    if (board == NULL) {
        return;
    }
    int counter= 0;
    while (counter < x) {
        const int i = rand() % boardSize;
        const int j = rand() % boardSize;
        if (board[i][j] != 0) {
            board[i][j] = 0;
            counter++;
        }
    }
}

int getNumberOfCellsToRemove(int boardSize, Difficulty difficulty) {
    switch (difficulty) {
        case EASY:
            if (boardSize == 4) return 5;
            if (boardSize == 9) return 41;
            if (boardSize == 16) return 116;
            break;
        case MEDIUM:
            if (boardSize == 4) return 8;
            if (boardSize == 9) return 48;
            if (boardSize == 16) return 141;
            break;
        case HARD:
            if (boardSize == 4) return 10;
            if (boardSize == 9) return 55;
            if (boardSize == 16) return 166;
            break;
        default:
            return 0;
    }
    return 0;
}

int **generateSudoku(Difficulty difficulty, int boardSize, Game *game) {
    int x = getNumberOfCellsToRemove(boardSize, difficulty);
    int **board = allocateBoard(boardSize);

    if (board == NULL) {
        return NULL;
    }
    // fill the board with zeros
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j] = 0;
        }
    }
    // 1. fill diagonal 3x3 boxes (upper left, middle, lower right)
    fillDiagonal(board, boardSize);

    // 2. fill remaining boxes on the board
    if (!fillRemaining(board, 0, 0, boardSize)) {
        freeGame(board, boardSize);
        return NULL;
    }

    // 3. copy solved sudoku to game->solution using copyBoard
    game->solution = allocateBoard(boardSize);
    if (!game->solution) {
        printf("Memory allocation failed for solution.\n");
        freeGame(board, boardSize);
        return NULL;
    }

    copyBoard(board, game->solution, boardSize);

    // 4. remove x random digits
    removeXDigits(board, x, boardSize);

    return board;
}




