#include "utils.h"
#include "sudoku.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void printBoard(Game *game, bool showSolution) {
    if (!game || !game->board || !game->solution) return;

    int size = game->boardSize;
    int boxSize = sqrt(size);
    int **boardToPrint = showSolution ? game-> solution : game -> board;

    // numerating columns (easier for user)
    printf("     \n");
    printf("     ");
    for (int i = 0; i < size; i++) {
        // +1 for 1-based indexing
        printf("%2d ", i+1);  // <-- tutaj %2d daje dwucyfrową szerokość
        if ((i + 1) % boxSize == 0 && i != size - 1) {
            printf("| ");
        }
    }
    printf("|\n");

    // top border
    printf("    ");
    for (int i = 0; i < size + boxSize - 1; i++) {
        printf("----");
    }
    printf("-\n");

    // rows
    for (int i = 0; i < size; i++) {
        // +1 for 1-based indexing
        printf("%2d | ", i+1);  // Numer wiersza
        for (int j = 0; j < size; j++) {
            if (boardToPrint[i][j] == 0)
                printf(" . ");
            else
                printf(" %X ", boardToPrint[i][j]);

            if ((j + 1) % boxSize == 0 && j != size - 1)
                printf("| ");
        }
        printf("|\n");

        // horizontal line between boxes
        if ((i + 1) % boxSize == 0 && i != size - 1) {
            printf("    ");
            for (int k = 0; k < size + boxSize - 1; k++) {
                printf("----");
            }
            printf("-\n");
        }
    }
}

// creating new game
Game *createNewGame(int size, Difficulty difficulty) {
    Game *game = malloc(sizeof(Game));
    if (!game) {
        printf("Memory allocation failed for Game.\n");
        return NULL;
    }

    game->boardSize = size;
    game->difficulty = difficulty;
    game->board = NULL;
    game->solution = NULL;

    return game;
}

// save game to text file
void saveGameToFile(Game *game, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file for saving.\n");
        return;
    }

    fprintf(file, "%d %d\n", game->boardSize, game->difficulty);

    for (int i = 0; i < game->boardSize; i++) {
        for (int j = 0; j < game->boardSize; j++) {
            fprintf(file, "%d ", game->board[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Game saved to %s successfully.\n", filename);
}

// loading existing game
Game *loadGameFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file for loading.\n");
        return NULL;
    }

    int size, difficulty;
    fscanf(file, "%d %d", &size, &difficulty);

    Game *game = malloc(sizeof(Game));
    game->boardSize = size;
    game->difficulty = difficulty;

    game->board = malloc(size * sizeof(int *));

//    if (!game) {
//        fclose(file);
//        return NULL;
//    }

    for (int i = 0; i < size; i++) {
        game->board[i] = malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            fscanf(file, "%d", &game->board[i][j]);
        }
    }

    fclose(file);
    printf("Game loaded from %s successfully.\n", filename);
    return game;
}

void freeGame(Game *game) {
    if (game == NULL) {
        return;
    }

    // free memory for the board
    for (int i = 0; i < game->boardSize; i++) {
        free(game->board[i]);
    }
    free(game->board);

    // free memory for solution (if exists)
    if (game->solution != NULL) {
        for (int i = 0; i < game-> boardSize; i++) {
            free(game->solution[i]);
        }
        free(game->solution);
    }

    // free 'game' struct itself
    free(game);
}

int **allocateBoard(int boardSize) {
    int **board = malloc(boardSize * sizeof(int *));
    if (board == NULL) {
        perror("Error allocating memory for board");
        return NULL;
    }
    for (int i = 0; i < boardSize; i++) {
        board[i] = (int *)malloc(boardSize * sizeof(int));
        if (board[i] == NULL) {
            perror("Error allocating memory for board");
            // free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(board[j]);
            }
            free(board);
            return NULL;
        }
    }
    return board;
}

// used for copying the sudoku's solution (user can choose whether he/she wants to see it or not)
void copyBoard(int **source, int **destination, int boardSize) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            destination[i][j] = source[i][j];
        }
    }
}

// function to make moves/save the game etc
void playGame(Game *game) {
    char input[100];
    int row, col, number;
    bool gameRunning = true;

    while (gameRunning) {
        system("clear");
        printBoard(game, false);

        printf("\nOptions:\n");
        printf("1. Make move (format: row col number)\n");
        printf("2. Show solution\n");
        printf("3. Save current game.\n");
        printf("4. Back to menu\n");
        printf("Your choice: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        switch (input[0]) {
            case '1': {
                printf("Enter row, column and number (e.g., '1 2 3'): ");
                if (fgets(input, sizeof(input), stdin) == NULL) break;

                if (sscanf(input, "%d %d %d", &row, &col, &number) != 3) {
                    printf("Invalid input format! Use: row col number\n");
                    break;
                }

                row--;
                col--;

                // Validate using existing functions
                if (row < 0 || row >= game->boardSize ||
                    col < 0 || col >= game->boardSize) {
                    printf("Invalid position! Use 1-%d\n", game->boardSize);
                    break;
                }

                if (number < 1 || number > game->boardSize) {
                    printf("Invalid number! Use 1-%d\n", game->boardSize);
                    break;
                }

                if (game->board[row][col] != 0) {
                    printf("Cell already filled!\n");
                    break;
                }

                if (!finalCheck(game->board, row, col, number, game->boardSize)) {
                    printf("Invalid move! Number conflicts with existing values.\n");
                    break;
                }

                // Make the move
                game->board[row][col] = number;

                // Check if game is complete
                if (isGameComplete(game)) {
                    printBoard(game, false);
                    printf("\nCONGRATULATIONS! You did it! :D\n");
                    gameRunning = false;
                }
                break;
            }
            case '2':
                // Show solution
                printf("\nSolution:\n");
                printBoard(game, true);
                printf("Press Enter to return to menu...");
                fgets(input, sizeof(input), stdin);
                return; // exit the game
            case '3':
                if (!game) {
                    printf("No game to save! :( Create a new game first.\n");
                    break;
                }
                char filename[100];
                printf("Enter filename to save: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0'; // remove newline

                saveGameToFile(game, filename);
                break;
            case '4':
                gameRunning = false;
                break;
            default:
                printf("Invalid option! Try again.\n");
        }
    }
}

bool isGameComplete(Game *game) {
    for (int i = 0; i < game->boardSize; i++) {
        for (int j = 0; j < game->boardSize; j++) {
            if (game->board[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

