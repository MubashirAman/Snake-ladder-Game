#include <iostream>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

int score = 0;
int color = 9;

void gotoRowCol(int rpos, int cpos) {
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

void sleep(int m) {
    Sleep(m);
}

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void game_info() {
    int row = 2, colm = 35;
    gotoRowCol(row, colm);
    cout << "Game details:   ";
    row += 2;
    gotoRowCol(row, colm);
    cout << "USE Arrow keys For Movement: ";
    row += 2;
    gotoRowCol(row, colm);
    cout << "Left Arrow to move Left ";
    row += 2;
    gotoRowCol(row, colm);
    cout << "Right Arrow to move Right ";
    row += 2;
    gotoRowCol(row, colm);
    cout << "UP And Down Arrows to change  Direction";
    row += 2;
}

char getArrowKey() {
    char ch = getch();
    if (ch == 0 || ch == -32) {
        ch = getch();
        switch (ch) {
            case 72:
                return 'U';
            case 80:
                return 'D';
            case 75:
                return 'L';
            case 77:
                return 'R';
        }
    }
    return ch;
}

void save_game(char board[30][30]) {
    ofstream save("game.txt");
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            save << board[i][j];
        }
        save << endl;
    }
    save << score;
}

void gboard(char board[30][30], int rsize, int csize) {
    char brdr = 219;
    for (int i = 0; i < rsize; i++) {
        for (int j = 0; j < csize; j++) {
            if (i == rsize - 1 || j == 0 || j == csize - 1) {
                board[i][j] = brdr;
            } else {
                board[i][j] = '.';
            }
        }
    }
}

void boardoutput(char board[30][30]) {
    int rsize = 30, csize = 30;
    char bdr = 219;
    for (int i = 0; i < rsize; i++) {
        gotoRowCol(0 + i, 0);
        for (int j = 0; j < csize; j++) {
            if (board[i][j] == bdr) {
                setColor(3);
                cout << board[i][j];
            } else {
                setColor(4);
                cout << board[i][j];
            }
        }
    }
}

void remove_shape(char arry[3][3], int row, int colm) {
    for (int i = 0; i < 3; i++) {
        gotoRowCol(row, colm);
        for (int j = 0; j < 3; j++) {
            setColor(4);
            cout << '.';
        }
        row++;
    }
}

void shape_output(char arry[3][3], int row, int colm, int color) {
    for (int i = 0; i < 3; i++) {
        gotoRowCol(row + i, colm);
        for (int j = 0; j < 3; j++) {
            setColor(color);
            cout << arry[i][j];
        }
    }
}

void shape_rotation(char arry[3][3]) {
    char rtdarry[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            rtdarry[j][2 - i] = arry[i][j];
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            arry[i][j] = rtdarry[i][j];
        }
    }
}

void g_control(char arry[3][3], char key, int& colm, int csize) {
    if (key == 'L' && colm > 1)
        colm = colm - 1;
    else if (key == 'R' && colm < csize - 4)
        colm = colm + 1;
    else if (key == 'U' || key == 'D')
        shape_rotation(arry);
}

bool shape_check(char board[30][30], char arry[3][3], int row, int colm) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (arry[i][j] != '.' && board[row + i][colm + j] != '.')
                return true;
        }
    }
    return false;
}

void updt_board(char board[30][30], char arry[3][3], int row, int colm) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (arry[i][j] != '.')
                board[i + row][j + colm] = arry[i][j];
        }
    }
    boardoutput(board);
}

void shape_movement(char board[30][30], char arry[3][3], int rsize, int csize, int color) {
    int row = 0, colm = 0, prerow = 0, precolm = 0;
    int clr = color;
    while (colm < 2) {
        colm = rand() % csize - 3;
    }
    while (row < rsize - 4) {
        prerow = row;
        precolm = colm;
        if (shape_check(board, arry, row + 1, colm))
            break;
        shape_output(arry, row, colm, color);
        sleep(500);
        if (kbhit()) {
            char key;
            key = getArrowKey();
            g_control(arry, key, colm, csize);
        }
        remove_shape(arry, prerow, precolm);
        row++;
        score += 1;
        gotoRowCol(14, 35);
        cout << "score:  " << score;
    }
    updt_board(board, arry, row, colm);
    cout << "\a";
    save_game(board);
}
void sqrshape(char board[30][30], int rsize, int csize) {
    color = 4;
    char fil = 177, sp = '.';
    char arry[3][3] = {
        {fil, fil, fil},
        {fil, fil, fil},
        {fil, fil, fil},
    };
    shape_movement(board, arry, rsize, csize, color);
}

void ishape(char board[30][30], int rsize, int csize) {
    char fil = 177, sp = '.';
    char arry[3][3] = {
        {fil, sp, sp},
        {fil, sp, sp},
        {fil, sp, sp}
    };
     color = 2;
    shape_movement(board, arry, rsize, csize, color);
}

void zshape(char board[30][30], int rsize, int csize) {
    char fil = 177, sp = '.';
    char arry[3][3] = {
        {fil, fil, sp},
        {sp, fil, sp},
        {sp, fil, fil}
    };
    color = 5;
    shape_movement(board, arry, rsize, csize, color);
}

void sshape(char board[30][30], int rsize, int csize) {
    char fil = 177, sp = '.';
    char arry[3][3] = {
        {sp, fil, fil},
        {sp, fil, sp},
        {fil, fil, sp}
    };
    color = 6;
    shape_movement(board, arry, rsize, csize, color);
}

void lshape(char board[30][30], int rsize, int csize) {
    char fil = 177, sp = '.';
    char arry[3][3] = {
        {fil, sp, sp},
        {fil, sp, sp},
        {fil, fil, fil}
    };
     color = 7;
    shape_movement(board, arry, rsize, csize, color);
}

void tshape(char board[30][30], int rsize, int csize) {
    char fil = 177, sp = '.';
    char arry[3][3] = {
        {fil, fil, fil},
        {sp, fil, sp},
        {sp, fil, sp}
    };
    color = 9;
    shape_movement(board, arry, rsize, csize, color);
}

void shapeturn(char board[30][30], int& num, int rsize, int csize) {
    switch (num) {
        case 1:
            sqrshape(board, rsize, csize);
            break;
        case 2:
            ishape(board, rsize, csize);
            break;
        case 3:
            zshape(board, rsize, csize);
            break;
        case 4:
            sshape(board, rsize, csize);
            break;
        case 5:
            lshape(board, rsize, csize);
            break;
        case 6:
            tshape(board, rsize, csize);
            break;
    }
}

void random_num(char board[30][30], int rsize, int csize, int& chk) {
    int num;
    while (1) {
        num = rand() % 6 + 1;
        shapeturn(board, num, rsize, csize);

        for (int j = 1; j < 19; j++) {
            if (board[0][j] != '.') {
                chk++;
            }
        }
        if (chk > 0)
            break;
    }
}

void load_game(char board[30][30], int rsize, int csize, int chk) {
    ifstream load("game.txt");
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            load >> board[i][j];
        }
    }
    boardoutput(board);
    game_info();
    load >> score;
    random_num(board, rsize, csize, chk);
}

int main() {
    char board[30][30], arry[3][3], choice;
    int rsize = 30, csize = 30, chk = 0;
    gotoRowCol(14, 50);
    cout << " Welcome to Tetris Game";
    gotoRowCol(16, 50);
    cout << " Do you want to load the game [y]   OR New Game [ N ]";
    cin >> choice;
    system("cls");
    if (choice == 'y' || choice == 'Y') {
        load_game(board, rsize, csize, chk);
    } else {
        gboard(board, rsize, csize);
        boardoutput(board);
        game_info();
        random_num(board, rsize, csize, chk);
    }
    gotoRowCol(10, 50);
    cout << "Game Over.......press any key to exit ";
    getch();
    return 0;
}
