# Console Tic-Tac-Toe (in C)

A classic Tic-Tac-Toe game built entirely in C. This project runs in the terminal and features multiple game modes, a persistent scoreboard, an unbeatable AI, and a clean, colorful interface.

This was a fun project to learn and implement C programming concepts, including array manipulation, functions, input/output handling, and basic AI algorithms.

This is my first project of college. so please support

## ✨ Features

* **Colorful Terminal Interface:** Uses ANSI escape codes for a visually appealing and easy-to-read board (e.g., 'X' is red, 'O' is cyan, the board is yellow).
* **Multiple Game Modes:**
    * Player vs. Player (PvP)
    * Player vs. Computer (Normal)
    * Player vs. Computer (Hard)
* **Smart AI Opponents:**
    * **Normal Mode:** A rules-based AI that checks for immediate wins or blocks.
    * **Hard Mode:** An unbeatable AI that uses the **Minimax algorithm** to determine the perfect move every time.
* **Persistent Scoreboard:** All game results (Player X, Player O, Computer, and Ties) are automatically saved to `tictactoe_scores.txt` and reloaded the next time you play.
* **Robust Input Handling:** Includes helper functions to prevent crashes from invalid inputs (like entering letters instead of numbers).

## 🚀 How to Compile and Run

This game is designed for Unix-like terminals (Linux, macOS) or modern Windows terminals (like Windows Terminal or Git Bash) that support ANSI color codes.

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/I-Am-Krishn/tictactoe-minimax-ai.git](https://github.com/I-Am-Krishn/tictactoe-minimax-ai.git)
    cd tictactoe-minimax-ai
    ```

2.  **Compile the code:**
    You'll need a C compiler like `gcc`.
    ```bash
    gcc main.c -o tictactoe
    ```

3.  **Run the game:**
    ```bash
    ./tictactoe
    ```

## 🎮 How to Play

1.  Run the executable.
2.  Choose your game mode (1-3) from the main menu.
3.  When it's your turn, enter a number from **1 to 9** to place your piece on the corresponding cell. The available cells are shown with dim numbers.
4.  The game will announce the winner or if it's a tie.
5.  After a game, you'll be asked if you want to play again.
6.  Press **Ctrl+C** at any time to quit.

## 💻 Screenshots
<img width="768" height="225" alt="image" src="https://github.com/user-attachments/assets/8f1ed350-ec7c-431e-bc36-ff000f6017c4" />
<img width="646" height="433" alt="image" src="https://github.com/user-attachments/assets/29a2dd60-6abb-414e-9845-abbbf5246099" />

## 👥 Author

This project was created by:
* **Krishn Dhola** ([I-Am-Krishn](https://github.com/I-Am-Krishn))
