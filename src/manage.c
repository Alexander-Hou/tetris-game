#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"
#include <stdbool.h>

// 检查当前分数是否超过最高分数
bool game_check_best_score(game *g){
        int best_score = game_load_best_score();
        return g->points > best_score;
}

// 保存最高分数到文件
void game_save_best_score(game *g){
    FILE *file = fopen(BEST_SCORE_FILE,"w");
    if (file != NULL) {
        if (game_check_best_score(g)){
            fprintf(file, "%d", g->best_score);
        }
        fclose(file);
    } else {
        perror("Failed to open best score file for writing");  // 无法打开文件进行写入
    }
}

// 从文件加载最高分数
int game_load_best_score(void){
    FILE *file = fopen(BEST_SCORE_FILE,"r");
    int best_score = 0;
    if (file != NULL) {
        if (fscanf(file, "%d", &best_score) != 1) {
            best_score = 0; // 如果读取失败，设置为0
        }
        fclose(file);
    } else {
        // 文件不存在，默认最高分数为0
        best_score = 0;
    }
    return best_score;
}

// 更新最高分
void game_update_best_score(game *g){
    int loaded_best = game_load_best_score();
    if (g->points > loaded_best) {
        g->best_score = g->points;  // 更新游戏中的best_score
        game_save_best_score(g);
    } else {
        g->best_score = loaded_best; // 保持文件中的最高分
    }
}