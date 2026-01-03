#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <windows.h> 
#include <string.h>
#include "tetris.h"

int main(int argc, char **argv) {
    bool restart = true;
    do {
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

        // 添加启动界面
        display_start_screen(g);

        // Windows控制台设置
        system("cls");
        printf("Starting Tetris...\n");
        Sleep(500);
        
        // 隐藏光标
        hide_cursor();
        
        // 游戏主循环
        while (running) {
            if (!paused) {
            // 执行游戏逻辑
            running = game_tick(g, mov);
            display_board(g);
            }
            
            // 处理输入
            mov = MOV_NONE;
            if (_kbhit()) {
                int ch = _getch();
                
                // 处理箭头键
                if (ch == 0 || ch == 0xE0) {
                    ch = _getch();
                    switch (ch) {
                        case 75: mov = MOV_LEFT; break;    
                        case 77: mov = MOV_RIGHT; break;   
                        case 72: mov = MOV_CLOCK; break;   
                        case 80: mov = MOV_DROP; break;    
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
                                printf("============ TETRIS ============\n");
                                printf("Score: %d  Level: %d  Lines: %d\n", g->points, g->level, g->lines_to_clear);
                                printf("Best Score: %d",g->best_score);
                                printf("\n");
                                printf("Hold: ");
                                if (g->block_stored != NULL) {
                                    // 显示Hold方块的类型
                                    display_hold_type(g);
                                } else {
                                    printf("[Empty]");
                                }
                                printf("   Next: ");
                                // 显示Next方块的类型
                                display_next_type(g);
                                printf("\n\n");
                                printf("\n=== GAME PAUSED ===\n");
                                printf("Press P to resume\n");
                                } else {
                                system("cls");  // 恢复游戏：只需清屏，正常循环会绘制游戏界面
                                }
                            break;
                        case 'r':
                        case 'R':
                            // 重新开始游戏
                            game_destroy(g); // 销毁当前游戏
                            g = game_create(22, 10); // 创建新游戏
                            paused = false; // 确保游戏不在暂停状态
                            // 添加时延效果
                            time_delay();
                            system("cls"); // 再次清屏，准备显示新游戏
                            running = true; // 确保游戏继续运行
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
        game_update_best_score(g);
        system("cls");
        printf("Game over!\n");
        printf("You finished with %d points on level %d.\n", g->points, g->level);
        printf("Your best score of the game is %d",g->best_score);
        // 清理
        game_destroy(g);
        // 添加询问是否重新开始
        printf("\n");
        printf("Thank you for playing Tetris!\n");
        printf("Press R to restart or any other key to exit.\n");
        // 等待用户输入
        int ch = _getch();
        if (ch == 'r' || ch == 'R') {
            restart = true;
            // 添加时延效果
            time_delay();
            system("cls");
        } else {
            restart = false;
        }
    } while (restart);
    return 0;
}