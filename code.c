# Tittactoe
# code réalisé par MIKOV Théodor BCS3 

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 3

void draw_board(char board[SIZE][SIZE]) {
    clear();
    int count = 1;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            char c = board[i][j];
            if (c == ' ')
                mvprintw(i * 2, j * 4, "%d", count);
            else
                mvprintw(i * 2, j * 4, "%c", c);
            if (j < SIZE - 1) mvprintw(i * 2, j * 4 + 1, "|");
            count++;
        }
        if (i < SIZE - 1) {
            mvprintw(i * 2 + 1, 0, "---------");
        }
    }
    refresh();
}

int check_winner(char board[SIZE][SIZE], char player) {
    for (int i = 0; i < SIZE; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return 1;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
            return 1;
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return 1;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return 1;

    return 0;
}

void ai_move(char board[SIZE][SIZE]) {
    int row, col;
    do {
        row = rand() % SIZE;
        col = rand() % SIZE;
    } while (board[row][col] != ' ');
    board[row][col] = 'O';
}

void reset_board(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = ' ';
}

int main(void) {
    char board[SIZE][SIZE];
    int row, col;
    char mode;
    char replay;
    int scoreX = 0;
    int scoreO = 0;

    srand(time(NULL));

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    do {
        int moves = 0;
        char player = 'X';
        reset_board(board);

        clear();
        mvprintw(0, 0, "Choisissez le mode : (1) Joueur vs Joueur  (2) Joueur vs IA");
        mode = getch();
        clear();

        while (1) {
            draw_board(board);
            mvprintw(SIZE * 2, 0, "Joueur %c, entrez un coup (1-9) ou 'q' pour quitter :", player);
            refresh();

            int input = getch();
            if (input == 'q') {
                endwin();
                return 0;
            }

            if (input >= '1' && input <= '9') {
                int pos = input - '1';
                row = pos / SIZE;
                col = pos % SIZE;

                if (board[row][col] == ' ') {
                    board[row][col] = player;
                    moves++;

                    if (check_winner(board, player)) {
                        draw_board(board);
                        mvprintw(SIZE * 2 + 1, 0, "Le joueur %c a gagné !", player);
                        if (player == 'X') scoreX++;
                        else scoreO++;
                        break;
                    }

                    if (moves == SIZE * SIZE) {
                        draw_board(board);
                        mvprintw(SIZE * 2 + 1, 0, "Match nul !");
                        break;
                    }

                    if (mode == '2' && player == 'X') {
                        player = 'O';
                        ai_move(board);
                        moves++;

                        if (check_winner(board, player)) {
                            draw_board(board);
                            mvprintw(SIZE * 2 + 1, 0, "Le joueur %c (IA) a gagné !", player);
                            scoreO++;
                            break;
                        }

                        if (moves == SIZE * SIZE) {
                            draw_board(board);
                            mvprintw(SIZE * 2 + 1, 0, "Match nul !");
                            break;
                        }
                    }

                    player = (player == 'X') ? 'O' : 'X';
                }
            }
        }

        // Affiche le score
        mvprintw(SIZE * 2 + 3, 0, "SCORE : X = %d | O = %d", scoreX, scoreO);

        mvprintw(SIZE * 2 + 5, 0, "Rejouer ? (O/N) : ");
        do {
            replay = getch();
        } while (replay != 'O' && replay != 'o' && replay != 'N' && replay != 'n');

    } while (replay == 'O' || replay == 'o');

    endwin();
    return 0;
}
