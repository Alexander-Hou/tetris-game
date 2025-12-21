#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <windows.h> 
#include <string.h>
#include "tetris.h"

int main(int argc, char **argv) {
    game *g;  // 游戏对象指针
    move mov = MOV_NONE;  // 当前玩家操作
    bool running = true;  // 游戏运行状态
    bool paused = false; // 游戏暂停状态
    
    // 创建新游戏
    g = game_create(22, 10);
    
    // 随机数种子调用
    srand((unsigned int)time(NULL));  
    
    // Windows控制台设置
    system("cls");
    printf("Starting Tetris...\n");
    Sleep(500);
    
    // 隐藏光标
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
    
    // 游戏主循环
    while (running) {
        if (!paused) {
        // 执行游戏逻辑
        running = game_tick(g, mov);
        // 显示游戏界面
        display_board(g);
        }
        
        // 处理输入
        mov = MOV_NONE;
        if (_kbhit()) {
            int ch = _getch();
            
            // 处理功能键（箭头键）
            if (ch == 0 || ch == 0xE0) {
                ch = _getch();
                switch (ch) {
                    case 75: mov = MOV_LEFT; break;    // 左箭头
                    case 77: mov = MOV_RIGHT; break;   // 右箭头
                    case 72: mov = MOV_CLOCK; break;   // 上箭头
                    case 80: mov = MOV_DROP; break;    // 下箭头
                }
            } else {
                // 处理普通按键
                switch (ch) {
                    case 'a':
                    case 'A':
                        mov = MOV_LEFT;
                        break;
                    case 'd':
                    case 'D':
                        mov = MOV_RIGHT;
                        break;
                    case 'w':
                    case 'W':
                        mov = MOV_CLOCK;
                        break;
                    case 's':
                    case 'S':
                        mov = MOV_DROP;
                        break;
                    case ' ':
                        mov = MOV_STORE;
                        break;
                    case 'p':
                    case 'P':
                        paused = !paused; // 切换暂停状态
                        if (paused) {
                            // 进入暂停：清屏并显示暂停界面
                            system("cls");
                            system("cls");
                            printf("============ TETRIS ============\n");
                            printf("Score: %d  Level: %d  Lines: %d\n", g->points, g->level, g->lines_to_clear);
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
                            printf("\n=== GAME PAUSED ===\n");
                            printf("Press P to resume\n");
                        } else {
                            // 恢复游戏：只需清屏，正常循环会绘制游戏界面
                            system("cls");
                        }
                        break;
                    case 'q':
                    case 'Q':
                        running = false;
                        break;
                }
            }
        }
        // 延迟
        Sleep(50);
    }
    // 游戏结束
    system("cls");
    printf("Game over!\n");
    printf("You finished with %d points on level %d.\n", g->points, g->level);
    // 清理
    game_destroy(g);
    printf("\nPress any key to exit...");
    _getch();
    return 0;
}