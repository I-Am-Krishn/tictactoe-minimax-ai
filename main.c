#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define CLR_RESET     "\x1b[0m"
#define CLR_BOLD      "\x1b[1m"
#define CLR_DIM       "\x1b[2m"
#define CLR_RED       "\x1b[31m"
#define CLR_CYAN      "\x1b[36m"
#define CLR_GREEN     "\x1b[32m"
#define CLR_YELLOW    "\x1b[33m"
#define CLR_BG_BLUE   "\x1b[44m"

char board[9];
int playerXWins = 0, playerOWins = 0, computerWins = 0, tieCount = 0;
int lastMoveIndex = -1;
int lastComputerMoveIndex = -1;

void loadScores();
void saveScores();

static void clearScreen() {
    printf("\x1b[2J\x1b[H");
}

static void printCellChar(char c) {
    if (c == 'X') printf(CLR_BOLD CLR_RED "X" CLR_RESET);
    else if (c == 'O') printf(CLR_BOLD CLR_CYAN "O" CLR_RESET);
    else printf(" ");
}

static void printGuideChar(int pos) {
    printf(CLR_DIM "%d" CLR_RESET, pos);
}

static void printHeader() {
    printf(CLR_BG_BLUE CLR_BOLD "   TIC-TAC-TOE   " CLR_RESET "\n");
    printf(CLR_DIM "  Use numbers 1-9 to play. Press Ctrl+C to quit anytime.\n\n" CLR_RESET);
}

static void printScoreBox() {
    printf(CLR_GREEN "┌───────────────────────────┐\n" CLR_RESET);
    printf(CLR_GREEN "│" CLR_RESET "  " CLR_BOLD "SCORES" CLR_RESET "                  " CLR_GREEN "│\n" CLR_RESET);
    printf(CLR_GREEN "├───────────────────────────┤\n" CLR_RESET);
    printf(CLR_GREEN "│ " CLR_RESET "Player X: %2d    Player O: %2d   \n" CLR_RESET, playerXWins, playerOWins);
    printf(CLR_GREEN "│ " CLR_RESET "Computer: %2d     Ties: %2d      \n" CLR_RESET, computerWins, tieCount);
    printf(CLR_GREEN "└───────────────────────────┘\n" CLR_RESET);
}

void drawBoard() {
    clearScreen();
    printHeader();
    printScoreBox();
    printf("\n");
    for (int row = 0; row < 3; row++) {
        printf("   ");
        for (int col = 0; col < 3; col++) printf(CLR_YELLOW "+---" CLR_RESET);
        printf(CLR_YELLOW "+" CLR_RESET "\n   ");
        for (int col = 0; col < 3; col++) {
            int idx = row * 3 + col;
            printf(CLR_YELLOW "|" CLR_RESET " ");
            if (board[idx] == ' ') printGuideChar(idx + 1);
            else {
                if (idx == lastComputerMoveIndex) {
                    printf(CLR_CYAN); printCellChar(board[idx]); printf(CLR_RESET);
                } else if (idx == lastMoveIndex) {
                    printf(CLR_RED); printCellChar(board[idx]); printf(CLR_RESET);
                } else printCellChar(board[idx]);
            }
            printf(" ");
        }
        printf(CLR_YELLOW "|" CLR_RESET "\n");
    }
    printf("   ");
    for (int col = 0; col < 3; col++) printf(CLR_YELLOW "+---" CLR_RESET);
    printf(CLR_YELLOW "+" CLR_RESET "\n\n");
}

void initGame() {
    for (int i = 0; i < 9; i++) board[i] = ' ';
    lastMoveIndex = -1;
    lastComputerMoveIndex = -1;
}

int isValid(int choice) {
    return (choice >= 1 && choice <= 9 && board[choice - 1] == ' ');
}

int win(char player) {
    int wins[][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };
    for (int i = 0; i < 8; i++)
        if (board[wins[i][0]] == player && board[wins[i][1]] == player && board[wins[i][2]] == player)
            return 1;
    return 0;
}

int isTie() {
    for (int i = 0; i < 9; i++) if (board[i] == ' ') return 0;
    return 1;
}

void computerMove() {
    int i;
    for (i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = 'O';
            if (win('O')) { lastComputerMoveIndex = i; return; }
            board[i] = ' ';
        }
    }
    for (i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = 'X';
            if (win('X')) { board[i] = 'O'; lastComputerMoveIndex = i; return; }
            board[i] = ' ';
        }
    }
    if (board[4] == ' ') { board[4] = 'O'; lastComputerMoveIndex = 4; return; }
    int corners[] = {0,2,6,8};
    for (i = 0; i < 4; i++) if (board[corners[i]] == ' ') { board[corners[i]] = 'O'; lastComputerMoveIndex = corners[i]; return; }
    int sides[] = {1,3,5,7};
    for (i = 0; i < 4; i++) if (board[sides[i]] == ' ') { board[sides[i]] = 'O'; lastComputerMoveIndex = sides[i]; return; }
}

int minimax(int depth, int isComputerTurn) {
    if (win('O')) return 10 - depth;
    if (win('X')) return -10 + depth;
    if (isTie()) return 0;
    int bestScore;
    if (isComputerTurn) {
        bestScore = -1000;
        for (int i = 0; i < 9; i++) {
            if (board[i] == ' ') {
                board[i] = 'O';
                int score = minimax(depth + 1, 0);
                board[i] = ' ';
                if (score > bestScore) bestScore = score;
            }
        }
    } else {
        bestScore = 1000;
        for (int i = 0; i < 9; i++) {
            if (board[i] == ' ') {
                board[i] = 'X';
                int score = minimax(depth + 1, 1);
                board[i] = ' ';
                if (score < bestScore) bestScore = score;
            }
        }
    }
    return bestScore;
}

int bestMoveHardIndex() {
    int bestScore = -1000, bestMoves[9], bestCount = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = 'O';
            int score = minimax(0, 0);
            board[i] = ' ';
            if (score > bestScore) {
                bestScore = score;
                bestCount = 0;
                bestMoves[bestCount++] = i;
            } else if (score == bestScore) {
                bestMoves[bestCount++] = i;
            }
        }
    }
    if (bestCount == 0) return -1;
    return bestMoves[rand() % bestCount];
}

void computerMoveHard() {
    int idx = bestMoveHardIndex();
    if (idx >= 0) { board[idx] = 'O'; lastComputerMoveIndex = idx; }
    else for (int i = 0; i < 9; i++) if (board[i] == ' ') { board[i] = 'O'; lastComputerMoveIndex = i; return; }
}

int read_integer_strict_noalpha(const char *prompt, int *out) {
    char buf[128];
    while (1) {
        if (prompt && prompt[0] != '\0') printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) return 0;
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') { buf[len-1] = '\0'; len--; }
        if (len == 0) { printf("❌ Error: empty input.\n"); continue; }
        int hasAlpha = 0;
        for (size_t i = 0; i < len; i++) if (isalpha((unsigned char)buf[i])) hasAlpha = 1;
        if (hasAlpha) { printf("❌ Error: enter numbers only.\n"); continue; }
        char *endptr;
        long val = strtol(buf, &endptr, 10);
        if (*endptr != '\0') { printf("❌ Invalid characters.\n"); continue; }
        *out = (int)val;
        return 1;
    }
}

int read_char_choice_strict(const char *prompt, char *out) {
    char buf[128];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) return 0;
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
        if (strlen(buf) != 1) { printf("❌ Enter single char (y/n).\n"); continue; }
        *out = buf[0];
        return 1;
    }
}

/* save only the single-line score (human-readable) */
void saveScores() {
    FILE *f = fopen("tictactoe_scores.txt", "w");
    if (!f) return;
    fprintf(f, "Player X: %d  |  Player O: %d  |  Computer: %d  |  Ties: %d\n",
            playerXWins, playerOWins, computerWins, tieCount);
    fclose(f);
}

/* load single-line score, robust fallback to zeros */
void loadScores() {
    FILE *f = fopen("tictactoe_scores.txt", "r");
    if (!f) { playerXWins = playerOWins = computerWins = tieCount = 0; return; }
    char line[256];
    if (!fgets(line, sizeof(line), f)) { playerXWins = playerOWins = computerWins = tieCount = 0; fclose(f); return; }
    fclose(f);
    int a,b,c,d;
    if (sscanf(line, "Player X: %d  |  Player O: %d  |  Computer: %d  |  Ties: %d", &a,&b,&c,&d) == 4) {
        playerXWins = a; playerOWins = b; computerWins = c; tieCount = d;
    } else {
        /* extract up to 4 integers if format differs */
        int vals[4] = {0,0,0,0}, vi = 0;
        char *p = line;
        while (*p && vi < 4) {
            if ((*p >= '0' && *p <= '9') || (*p == '-' && isdigit((unsigned char)p[1]))) {
                char *endptr;
                long v = strtol(p, &endptr, 10);
                vals[vi++] = (int)v;
                p = endptr;
            } else p++;
        }
        if (vi == 4) { playerXWins = vals[0]; playerOWins = vals[1]; computerWins = vals[2]; tieCount = vals[3]; }
        else { playerXWins = playerOWins = computerWins = tieCount = 0; }
    }
}

void playPvP() {
    char turn = 'X';
    int move;
    while (1) {
        drawBoard();
        if (!read_integer_strict_noalpha("Player (1-9): ", &move)) return;
        if (!isValid(move)) { printf("❌ Invalid move.\n"); continue; }
        board[move-1] = turn;
        if (turn == 'X') lastMoveIndex = move-1; else lastComputerMoveIndex = move-1;
        if (win(turn)) {
            drawBoard();
            if (turn == 'X') playerXWins++; else playerOWins++;
            saveScores();
            printf("🎉 Player %c wins!\n", turn);
            getchar();
            return;
        }
        if (isTie()) { drawBoard(); tieCount++; saveScores(); printf("🤝 Tie game!\n"); getchar(); return; }
        turn = (turn == 'X') ? 'O' : 'X';
    }
}

void playPvC() {
    int move;
    while (1) {
        drawBoard();
        if (!read_integer_strict_noalpha("You (1-9): ", &move)) return;
        if (!isValid(move)) { printf("❌ Invalid move.\n"); continue; }
        board[move-1] = 'X'; lastMoveIndex = move-1;
        if (win('X')) { drawBoard(); playerXWins++; saveScores(); printf("🌟 You win! 🌟\n"); getchar(); return; }
        if (isTie()) { drawBoard(); tieCount++; saveScores(); printf("🤝 Tie game!\n"); getchar(); return; }
        computerMove();
        if (win('O')) { drawBoard(); computerWins++; saveScores(); printf("💻 Computer wins!\n"); getchar(); return; }
        if (isTie()) { drawBoard(); tieCount++; saveScores(); printf("🤝 Tie game!\n"); getchar(); return; }
    }
}

void playPvC_Hard() {
    int move;
    while (1) {
        drawBoard();
        if (!read_integer_strict_noalpha("You (1-9): ", &move)) return;
        if (!isValid(move)) { printf("❌ Invalid move.\n"); continue; }
        board[move-1] = 'X'; lastMoveIndex = move-1;
        if (win('X')) { drawBoard(); playerXWins++; saveScores(); printf("🌟 You win! 🌟\n"); getchar(); return; }
        if (isTie()) { drawBoard(); tieCount++; saveScores(); printf("🤝 Tie game!\n"); getchar(); return; }
        computerMoveHard();
        if (win('O')) { drawBoard(); computerWins++; saveScores(); printf("💻 Computer (Hard) wins!\n"); getchar(); return; }
        if (isTie()) { drawBoard(); tieCount++; saveScores(); printf("🤝 Tie game!\n"); getchar(); return; }
    }
}

int main() {
    srand((unsigned int)time(NULL));
    loadScores();

    FILE *f = fopen("tictactoe_scores.txt", "r");
    if (f) {
        char buf[256];
        if (fgets(buf, sizeof(buf), f))
            printf("Loaded scores: %s\n", buf);
        fclose(f);
    }

    int mode;
    char playAgain = 'y';

    do {
        initGame();
        printf("\n");
        printf(CLR_BG_BLUE CLR_BOLD "   TIC-TAC-TOE   " CLR_RESET "\n\n");
        printf("1. Player vs Player\n");
        printf("2. Player vs Computer (Normal)\n");
        printf("3. Player vs Computer (Hard)\n");
        printf("4. Exit Game\n");
        if (!read_integer_strict_noalpha("Enter 1-4: ", &mode)) mode = 4;

        if (mode == 1) playPvP();
        else if (mode == 2) playPvC();
        else if (mode == 3) playPvC_Hard();
        else if (mode == 4) { printf("\nExiting the game...\n"); break; }
        else continue;

        printf("\n📊 SCORES:\n");
        printf("Player X: %d  |  Player O: %d  |  Computer: %d  |  Ties: %d\n",
               playerXWins, playerOWins, computerWins, tieCount);

        if (!read_char_choice_strict("\nWanna play again? (y/n): ", &playAgain)) break;
    } while (playAgain == 'y' || playAgain == 'Y');

    saveScores();
    printf("\nThanks for playing!\n");
    printf("\nCreated by Krishn Dhola\n");
    return 0;
}
