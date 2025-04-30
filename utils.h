//
// Created by Yola M on 30/04/2025.
//

#ifndef UTILS_H
#define UTILS_H

typedef enum {
    EASY,
    MEDIUM,
    HARD
} Difficulty;

typedef struct {
    int **board;
    int boardSize;
    int **solution;
    Difficulty difficulty;
} Game;


void printBoard(Game *game, bool showSolution);
Game *createNewGame(int size, Difficulty difficulty);
void saveGameToFile(Game *game, const char *filename);
Game *loadGameFromFile(const char *filename);
void freeGame(Game *game);
void playGame(Game *game);
bool isGameComplete(Game* game);

#endif //UTILS_H