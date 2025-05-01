//
// Created by Yola M on 28/04/2025.
//
//
// Created by Yola M on 28/04/2025.
//

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    // ensure getting different random number every time the program runs
    // initialization
    srand(time(NULL));
    Game *currentGame = NULL;
    int running = 1;
    int option;
    char input[100];

    // menu
    while (running) {
        printf("********************************\n");
        printf("*       WELCOME TO SUDOKU!     *\n");
        printf("********************************\n");
        printf("\nMain Menu:\n");
        printf("--------------\n");
        printf("  1. New game.\n");
        printf("  2. Launch existing game.\n");
        printf("  3. Tutorial.\n");
        printf("  4. Save current game.\n");
        printf("  5. Exit.\n");

        printf("Please enter an option from the main menu: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }

        if (sscanf(input, "%d", &option) != 1) {
            printf("Invalid input. Please enter a number 1-5.\n");
            continue;
        }

        switch (option) {
            case 1: {
                // board size selection
                int boardSize;
                Difficulty difficulty;
                char sizeInput[10], difficultyInput[10];

                printf("\n*** Choose Sudoku's board size.");
                printf("\nWrite '4' for a mini Sudoku - 4x4 board.");
                printf("\nWrite '9' for standard Sudoku - 9x9 board.");
                printf("\nWrite '16' for a super Sudoku - 16x16 board.");
                printf("\nBoard size choice: ");
                if (fgets(sizeInput, sizeof(sizeInput), stdin) == NULL) {
                    printf("Error reading input\n");
                    continue;
                }

                if (sscanf(sizeInput, "%d", &boardSize) != 1 ||
                    (boardSize != 4 && boardSize != 9 && boardSize != 16)) {
                        printf("Invalid size! Please enter 4, 9 or 16.\n");
                        continue;
                    }

                // difficulty selection
                printf("\n*** Select Difficulty ***\n");
                printf("[ Difficulty is based on the number of hints you get :) ]\n");
                printf("  a. Easy\n");
                printf("  b. Medium\n");
                printf("  c. Hard\n");
                printf("Difficulty choice: ");

                if (fgets(difficultyInput, sizeof(difficultyInput), stdin) == NULL) {
                    printf("Error reading input\n");
                    continue;
                }
                difficultyInput[strcspn(difficultyInput, "\n")] = '\0'; // Remove newline
                if (strlen(difficultyInput) != 1) {
                    printf("Please enter exactly one character (a/b/c).\n");
                    continue;
                }

                switch (difficultyInput[0]) {
                    case 'a': case 'A': difficulty = EASY; break;
                    case 'b': case 'B': difficulty = MEDIUM; break;
                    case 'c': case 'C': difficulty = HARD; break;
                    default:
                        printf("Invalid choice! Please enter a, b or c.\n");
                        continue;
                }

                currentGame = createNewGame(boardSize, difficulty);
                currentGame->board = generateSudoku(difficulty, boardSize, currentGame);
                printBoard(currentGame, false);
                playGame(currentGame);

                break;
            }
            case 2: {
                char filename[100];
                printf("Enter filename to load: \n");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0'; // remove newline

                if (currentGame) {
                    freeGame(currentGame->board, currentGame->boardSize);
                    freeGame(currentGame->solution, currentGame->boardSize);
                    free(currentGame);
                }
                currentGame = loadGameFromFile(filename);

                if (currentGame == NULL) {
                    printf("Game couldn't be loaded :(\n");
                }
                else {
                    printf("\nGame loaded successfully!\n");
                    printBoard(currentGame, false);
                    playGame(currentGame);
                }
                break;
            }
            case 3: {
                printf("\n< WELCOME TO SUDOKU TUTORIAL! >\n");
                printf("\nSUDOKU RULES:\n");
                printf("--------------\n");
                printf("1. You can only use digits from 1-9 (on 9x9 board), 1-4 (on 4x4 board) and 1-16 (on 16x16 board)\n");
                printf("2. Each row must contain numbers from 1-9/1-4/1-16 WITHOUT REPETITION.\n");
                printf("3. Each column must contain numbers from 1-9/1-4/1-16 WITHOUT REPETITION.\n");
                printf("4. Each number in 2x2 (4x4 board) block / 3x3 (9x9 board) block /4x4 (16x16 board) block can be used only once.\n");
                printf("5. Game ends when the whole Sudoku grid is correctly filled with numbers.\n");
                printf("\nSUDOKU BOARD SIZE:\n");
                printf("--------------\n");
                printf("* 9x9 board - classic board size for Sudoku. Split into 3x3 blocks.\n");
                printf("* 4x4 board - Mini Sudoku board size. Split into 2x2 blocks.\n");
                printf("* 16x16 board - Super Sudoku board size. Split into 4x4 blocks.\n");
                printf("\nSUDOKU DIFFICULTY:\n");
                printf("--------------\n");
                printf("Difficulty is based on the number of hints you get from the start. You have 3 options - easy/medium/hard mode.\n");
                break;
            }
            case 4: {
                if (!currentGame) {
                    printf("No game to save! :( Create a new game first.\n");
                    break;
                }
                char filename[100];
                printf("Enter filename to save: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0'; // remove newline

                saveGameToFile(currentGame, filename);
                printf("Game saved to %s\n", filename);
                break;
            }
            case 5: {
                running = 0;
                if (currentGame) {
                    freeGame(currentGame->board, currentGame->boardSize);
                    freeGame(currentGame->solution, currentGame->boardSize);
                    free(currentGame);
                }
                printf("See you next time! :)\n");
                break;
            }
            default: {
                printf("Invalid option!\n");
            }
        }
    }

    return 0;
}