# 🧩 ---- Sudoku Game in C ---- 🧩

This project is a console-based Sudoku game written in C, which allows players to generate Sudoku boards of 3 available sizes and 3 difficulty levels.

## 🕹️ Game Rules 🕹️
Sudoku is a classic number puzzle where the objective is to fill a square board with digits from 1 to n (n = board size) following these rules:
	•	Each digit must appear exactly once in each row
	•	Each digit must appear exactly once in each column
	•	Each digit must appear exactly once in each √n × √n subgrid (box)

e.g. A standard 9×9 board is divided into 3×3 subgrids (boxes). At the start of the game, some cells are already filled in — these are the hints, and the player’s task is to complete the rest of the board according to the rules.

## ✨ Features ✨
- 3 DIFFICULTY LEVELS:
  * EASY
  * MEDIUM
  * HARD (...16x16 board size is harder itself ;))
- 3 BOARD SIZES:
  * 4x4 (split into 2x2 boxes)
  * 9x9 (split into 3x3 boxes)
  * 16x16 (split into 4x4 boxes)
- INTERACTIVE MAIN MENU:
  1. New game.
  2. Launch existing game.
  3. Tutorial.
  4. Save current game.
  5. Exit.
