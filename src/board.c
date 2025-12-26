#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <windows.h> 
#include <string.h>
#include "tetris.h"

// Windows控制台颜色定义
#define COLOR_BLACK       0
#define COLOR_BLUE        FOREGROUND_BLUE                    // 1
#define COLOR_GREEN       FOREGROUND_GREEN                   // 2
#define COLOR_CYAN        (FOREGROUND_BLUE | FOREGROUND_GREEN) // 3
#define COLOR_RED         FOREGROUND_RED                     // 4
#define COLOR_MAGENTA     (FOREGROUND_RED | FOREGROUND_BLUE)  // 5
#define COLOR_YELLOW      (FOREGROUND_RED | FOREGROUND_GREEN) // 6
#define COLOR_WHITE       (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) // 7
#define COLOR_INTENSITY   FOREGROUND_INTENSITY               // 8

// 字符定义：使用ASCII字符
#define BLOCK_CHAR '#'    // 方块字符
#define EMPTY_CHAR '.'    // 空格字符

// 控制台颜色设置函数
static void set_console_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // 获取控制台句柄
    SetConsoleTextAttribute(hConsole, color);  // 设置文本颜色
}
// 重置控制台颜色为默认白色
static void reset_console_color() {
    set_console_color(COLOR_WHITE | COLOR_INTENSITY);  // 重置为白色
}
// 设置光标位置函数
static void set_cursor_position(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // 获取控制台句柄
    COORD pos = {x, y};  // 设置光标位置结构体
    SetConsoleCursorPosition(hConsole, pos);  // 设置光标位置
}
// 显示启动界面
void display_start_screen(game *g) {
    system("cls"); // 清屏
    printf("\n\n\n");
    printf("            ***********************************\n");
    printf("                     TETRIS GAME V1.0         \n");
    printf("            ***********************************\n");
    printf("\n");
    printf("          _______ ______  _______  _____   _  _____ \n");
    printf("         |__   __|  ____||__   __||  __ \\ | |/ ____|\n");
    printf("            | |  | |__      | |   | |__) || | (___  \n");
    printf("            | |  |  __|     | |   |  _  / | |\\___ \\ \n");
    printf("            | |  | |____    | |   | | \\ \\ | | ___) |\n");
    printf("            |_|  |______|   |_|   |_|  \\_\\|_||____/ \n");
    printf("\n\n\n");
    printf("             +-------- CONTROL KEYS --------+\n");
    printf("             |                              |\n");
    printf("             |   Move Left:   <- or A       |\n");
    printf("             |   Move Right:  -> or D       |\n");
    printf("             |   Rotate:       ^ or W       |\n");
    printf("             |   Hard Drop:    v or S       |\n");
    printf("             |   Hold Piece:    SPACE       |\n");
    printf("             |   Pause/Resume:   P          |\n");
    printf("             |   Quit Game:      Q          |\n");
    printf("             |   Restart Game:   R          |\n");
    printf("             |                              |\n");
    printf("             +------------------------------+\n");
    printf("\n");
    printf("                 Your best score is %d\n",g->best_score);
    printf("              Try to chanllenge yourself now!\n");
    printf("\n");
    printf("            ***********************************\n");
    printf("                   Press any key to play       \n");
    printf("            ***********************************\n");
    
    _getch(); // 等待按键开始游戏
}
// 终端光标隐藏函数
void hide_cursor(void) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
// 时延效果
void time_delay(void) {
    system("cls");
    printf("Restarting game in 3...\n");
    Sleep(1000);
    system("cls");
    printf("Restarting game in 2...\n");
    Sleep(1000);
    system("cls");
    printf("Restarting game in 1...\n");
    Sleep(1000);
}
// 显示Hold方块类型
void display_hold_type(game *g) {
    switch(g->block_stored->typ) {
        case TET_I: printf("[I]"); break;
        case TET_J: printf("[J]"); break;
        case TET_L: printf("[L]"); break;
        case TET_O: printf("[O]"); break;
        case TET_S: printf("[S]"); break;
        case TET_T: printf("[T]"); break;
        case TET_Z: printf("[Z]"); break;
        default: printf("[?]"); break;
    }
}
// 显示Next方块类型
void display_next_type(game *g) {
    switch(g->next_block) {
        case TET_I: printf("[I]"); break;
        case TET_J: printf("[J]"); break;
        case TET_L: printf("[L]"); break;
        case TET_O: printf("[O]"); break;
        case TET_S: printf("[S]"); break;
        case TET_T: printf("[T]"); break;
        case TET_Z: printf("[Z]"); break;
        default: printf("[?]"); break;
    }
}
// 显示游戏板
void display_board(game *g) {
    set_cursor_position(0, 0);  // 将光标移动到左上角
    printf("============ TETRIS ============\n");
    printf("Score: %d  Level: %d  Lines: %d\n", g->points, g->level, g->lines_to_clear);
    printf("Best Score: %d",g->best_score);
    printf("\n");
    printf("Hold: ");
    if (g->block_stored != NULL) {
        // 显示Hold方块的类型
        switch(g->block_stored->typ) {
            case TET_I: printf("[I]"); break;
            case TET_J: printf("[J]"); break;
            case TET_L: printf("[L]"); break;
            case TET_O: printf("[O]"); break;
            case TET_S: printf("[S]"); break;
            case TET_T: printf("[T]"); break;
            case TET_Z: printf("[Z]"); break;
            default: printf("[?]"); break;
        }
    } else {
        printf("[Empty]");
    }
    printf("   Next: ");
    // 显示Next方块的类型
    switch(g->next_block) {
        case TET_I: printf("[I]"); break;
        case TET_J: printf("[J]"); break;
        case TET_L: printf("[L]"); break;
        case TET_O: printf("[O]"); break;
        case TET_S: printf("[S]"); break;
        case TET_T: printf("[T]"); break;
        case TET_Z: printf("[Z]"); break;
        default: printf("[?]"); break;
    }
    printf("\n\n");
    // 游戏区域上边框
    printf("+");
    for (int j = 0; j < g->cols; j++){
        printf("--");
    }
    printf("+\n");
    // 创建临时数组来合并 board 和当前下落方块
    cell display_board[g->rows][g->cols];
    
    // 复制 board 内容
    for (int i = 0; i < g->rows; i++) {
        for (int j = 0; j < g->cols; j++) {
            display_board[i][j] = g->board[i][j];
        }
    }
    
    // 添加当前下落方块到临时数组
    if (g->block_dropped != NULL) {
        for (int i = 0; i < TETRIS; i++) {
            location pos = shape[g->block_dropped->typ][g->block_dropped->ori][i];  // 获取方块的相对位置
            int board_row = g->block_dropped->loc.row + pos.row;  // 计算在游戏板上的行位置
            int board_col = g->block_dropped->loc.col + pos.col;  // 计算在游戏板上的列位置
            
            // 检查位置是否在有效范围内
            if (board_row >= 0 && board_row < g->rows && 
                board_col >= 0 && board_col < g->cols) {
                cell cell_type = (cell)(g->block_dropped->typ + 1);
                display_board[board_row][board_col] = cell_type;
            }
        }
    }
    // 游戏区域内容
    for (int i = 0; i < g->rows; i++) {
        printf("|");
        for (int j = 0; j < g->cols; j++) {
            cell c = display_board[i][j];
            if (c != EMPTY) {
                // 根据单元格类型设置颜色
                switch (c) {
                    case CELL_I: set_console_color(COLOR_CYAN | COLOR_INTENSITY); break;     // 青色
                        case CELL_J: set_console_color(COLOR_BLUE | COLOR_INTENSITY); break;      // 蓝色
                        case CELL_L: set_console_color(COLOR_RED | FOREGROUND_GREEN); break;     // 橙色（红+绿）
                        case CELL_O: set_console_color(COLOR_YELLOW | COLOR_INTENSITY); break;   // 黄色
                        case CELL_S: set_console_color(COLOR_GREEN | COLOR_INTENSITY); break;    // 绿色
                        case CELL_T: set_console_color(COLOR_MAGENTA | COLOR_INTENSITY); break;  // 紫色
                        case CELL_Z: set_console_color(COLOR_RED | COLOR_INTENSITY); break;      // 红色
                        default: set_console_color(COLOR_WHITE); break;
                }
                printf("%c%c", BLOCK_CHAR, BLOCK_CHAR);  // 打印方块
                reset_console_color();  // 重置颜色
            } else {
                printf("%c%c", EMPTY_CHAR, EMPTY_CHAR);  // 打印空格
            }
        }
        printf("|\n");  // 行尾边框
    }
    
    // 游戏区域下边框
    printf("+");
    for (int j = 0; j < g->cols; j++) printf("--");
    printf("+\n");
}

/* 检查指定位置是否在棋盘范围内 */
bool game_is_valid_position(const game *g, int row, int col){
    return (row >= 0 && row < g->rows && col >= 0 && col < g->cols);
}
/* 获取指定位置的单元格状态 */
cell game_get_cell_status(const game *g, int row, int col){
    if (!game_is_valid_position(g, row, col)) {
        return EMPTY;  // 如果位置无效，返回EMPTY
    }
    return g->board[row][col];  // 返回指定位置的单元格状态
} 
/* 设置指定位置的单元格状态 */
void game_set_cell_status(game *g, int row, int col, cell value){
    if (!game_is_valid_position(g, row, col)) {
        return;  // 如果位置无效，直接返回
    }
    g->board[row][col] = value;  // 设置指定位置的单元格状态
}