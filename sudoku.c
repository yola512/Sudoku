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


void generateSudoku(Game *game) {
    if (!game) return;

    int boardSize = game->boardSize;
    int emptyCells;

    switch (game->difficulty) {
        case EASY:
            emptyCells = boardSize * boardSize * 0.3;
            break;
        case MEDIUM:
            emptyCells = boardSize * boardSize * 0.5;
            break;
        case HARD:
            emptyCells = boardSize * boardSize * 0.65;
            break;
        default:
            emptyCells = boardSize * boardSize * 0.5;
            break;
    }

    // Allocate memory
    game->solution = (int **)malloc(game->boardSize * sizeof(int *));
    if (!game->solution) return false;

    for (int i = 0; i < game->boardSize; i++) {
        game->solution[i] = (int *)calloc(game->boardSize, sizeof(int));
        if (!game->solution[i]) {
            for (int j = 0; j < i; j++) free(game->solution[j]);
            free(game->solution);
            return false;
        }
    }

    // Generate solution
    fillDiagonal(game->solution, game->boardSize);
    if (!fillRemaining(game->solution, 0, 0, game->boardSize)) {
        for (int i = 0; i < game->boardSize; i++) free(game->solution[i]);
        free(game->solution);
        return false;
    }

    // Allocate and copy to playable board
    game->board = (int **)malloc(game->boardSize * sizeof(int *));
    if (!game->board) {
        for (int i = 0; i < game->boardSize; i++) free(game->solution[i]);
        free(game->solution);
        return false;
    }

    for (int i = 0; i < game->boardSize; i++) {
        game->board[i] = (int *)malloc(game->boardSize * sizeof(int));
        if (!game->board[i]) {
            for (int j = 0; j < i; j++) free(game->board[j]);
            free(game->board);
            for (int j = 0; j < game->boardSize; j++) free(game->solution[j]);
            free(game->solution);
            return false;
        }
        memcpy(game->board[i], game->solution[i], game->boardSize * sizeof(int));
    }

    // // 2. copy solution to board before removing digits
    // for (int i = 0; i < game->boardSize; i++) {
    //     memcpy(game->board[i], game->solution[i], game->boardSize * sizeof(int));
    // }

    // 3. remove digits to create the puzzle
    removeXDigits(game->board, emptyCells, game->boardSize);
    return true;
}




