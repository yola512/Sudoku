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
int **allocateBoard(int boardSize);
void playGame(Game *game);
bool isGameComplete(Game* game);
void copyBoard(int **source, int **destination, int boardSize);
#endif //UTILS_H