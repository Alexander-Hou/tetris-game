#include "tetris.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

// 方块形状定义（三维数组）
location shape[NUM_TYPES][NUM_ORIENTATIONS][TETRIS] = {
    // I (长条形)
    {{{0, 1}, {1, 1}, {2, 1}, {3, 1}},    // 状态0
     {{2, 0}, {2, 1}, {2, 2}, {2, 3}},    // 状态1
     {{0, 2}, {1, 2}, {2, 2}, {3, 2}},    // 状态2
     {{1, 0}, {1, 1}, {1, 2}, {1, 3}}},   // 状态3   // 垂直
    // J
    {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},    // L型
     {{0, 1}, {1, 1}, {2, 0}, {2, 1}},    // 旋转90
     {{1, 0}, {1, 1}, {1, 2}, {2, 2}},    // 旋转180
     {{0, 0}, {0, 1}, {1, 0}, {2, 0}}},   // 旋转270
    // L
    {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},    // 镜像L
     {{0, 0}, {1, 0}, {2, 0}, {2, 1}},    // 旋转90
     {{1, 0}, {1, 1}, {1, 2}, {2, 0}},    // 旋转180
     {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},   // 旋转270
    // O (正方形)
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
    // S
    {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
     {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
     {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
     {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
    // T
    {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
     {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
     {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
     {{0, 1}, {1, 0}, {1, 1}, {2, 1}}},
    // Z
    {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
     {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
     {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
     {{0, 1}, {1, 0}, {1, 1}, {2, 0}}}
};
// 重力速度表（每个等级对应的下落速度，单位：毫秒）
int GRAVITY_LEVEL[MAX_LEVEL + 1] = {
    1000, 850, 720, 610, 520, 440, 380, 320, 270, 230,
    200, 170, 140, 120, 100, 85, 70, 60, 50, 40
};
// 消行得分表（每次消除对应的得分）
int SCORE_TABLE[5] = {0, 100, 250, 500, 1000};

// 创建并初始化游戏对象
game* game_create(int rows, int cols){
    game *g = (game *)malloc(sizeof(game));  // 分配内存
    if (!g) return NULL; // 检查内存分配是否成功
    game_init(g, rows, cols);
    return g;
}
// 初始化游戏对象
void game_init(game *g, int rows, int cols){
    g->rows = rows;
    g->cols = cols;
    // 分配游戏板内存
    g->board = (cell **)malloc(rows * sizeof(cell *));
    for (int i = 0; i < rows; i++) {
        g->board[i] = (cell *)malloc(cols * sizeof(cell));
        memset(g->board[i], EMPTY, cols * sizeof(cell));  // 初始化为空
    }
    // 初始化当前方块和保留方块
    g->block_dropped = NULL;
    g->block_stored = NULL;
    g->store_used = false;
    // 初始化游戏状态
    g->next_block = game_random_block();
    g->level = 0;
    g->points = 0;
    g->lines_to_clear = LINES_PER_LEVEL;  // 初始升级所需行数
    // 生成第一个下落方块
    g->next_block = game_random_block();
    game_create_new_block(g); // 生成第一个下落方块
}
// 销毁游戏对象
void game_destroy(game *g){
    // 释放游戏板内存
    for (int i = 0; i < g->rows; i++) {
        free(g->board[i]);
    }
    free(g->board);
    // 释放当前下落方块内存
    if (g->block_dropped) {
        free(g->block_dropped);
    }
    // 释放保留方块内存
    if (g->block_stored) {
        free(g->block_stored);
    }
    free(g);  // 释放游戏对象内存
}
// 处理游戏的每个tick，返回游戏是否继续
bool game_tick(game *g, move mov){
    switch(mov){
        case MOV_LEFT:
            game_move_left(g); // 处理左移逻辑
            break;
        case MOV_RIGHT:
            game_move_right(g); // 处理右移逻辑
            break;
        case MOV_CLOCK:
            game_clock_roll(g); // 处理顺时针旋转逻辑
            break;
        case MOV_DROP:
            game_drop(g); // 处理快速下落逻辑
            break;
        case MOV_STORE:
            game_store(g); // 处理保留方块逻辑
            break;
        case MOV_NONE:
        default:
            break;
    }
    bool can_drop = true; // 假设游戏继续
    for (int i = 0; i < TETRIS; i++) {
        location pos = shape[g->block_dropped->typ][g->block_dropped->ori][i];  // 获取方块的每个单元格位置
        int new_row = g->block_dropped->loc.row + pos.row + 1; // 下落一行后的行位置
        int new_col = g->block_dropped->loc.col + pos.col; // 列位置
        // 检查是否碰撞到底部或其他方块
        if (new_row >= g->rows || (game_is_valid_position(g,new_col,new_row)&&game_get_cell_status(g,new_row,new_col)!=EMPTY)) {
            can_drop = false; // 碰撞，游戏可能结束
            break;
    }
}
    if (can_drop) {
        g->block_dropped->loc.row += 1; // 方块下落一行
        return true; // 游戏继续
    } else {
        // 固定方块到游戏板
        for (int i = 0; i < TETRIS; i++) {
            location pos = shape[g->block_dropped->typ][g->block_dropped->ori][i];  // 获取方块的每个单元格位置
            int board_row = g->block_dropped->loc.row + pos.row; // 计算在游戏板上的行位置
            int board_col = g->block_dropped->loc.col + pos.col; // 计算在游戏板上的列位置
            if (game_is_valid_position(g, board_row, board_col)) {
                cell cell_type = (cell)(g->block_dropped->typ + 1); // 固定方块
                game_set_cell_status(g, board_row, board_col, cell_type); // 设置单元格状态
            }
        }
        // 检查并消除完整的行
        int lines_cleared = game_check_lines(g);
        if (lines_cleared > 0) {
            game_update_grade(g, lines_cleared); // 更新分数和等级
        }
        // 生成新的下落方块
        game_create_new_block(g);
        // 重置保留标志
        g->store_used = false; 
        // 检查游戏是否结束
        if (game_is_over(g)) {
            return false; // 游戏结束
        }
        return true; // 游戏继续
    }
}

// 检查游戏是否结束  
bool game_is_over(const game *g){ 
    // 如果新方块生成位置被占用，游戏结束
    location spawn_loc = {0, g->cols / 2 - 2}; // 假设方块从顶部中间生成
    for (int i = 0; i < TETRIS; i++) {
        location cell_loc = shape[g->next_block][0][i];
        int board_row = spawn_loc.row + cell_loc.row;
        int board_col = spawn_loc.col + cell_loc.col;
        if (board_row >= 0 && board_row < g->rows &&
            board_col >= 0 && board_col < g->cols) {
            if (g->board[board_row][board_col] != EMPTY) {
                return true; // 游戏结束
            }
        }
    }
    return false; // 游戏未结束
}
// 生成新的下落方块
void game_create_new_block(game *g){
    tetris *new_block = (tetris *)malloc(sizeof(tetris)); // 分配新的方块内存
    new_block->typ = g->next_block; // 设置方块类型
    new_block->ori = 0; // 初始旋转方向
    new_block->loc.row = 0; // 初始行位置
    new_block->loc.col = g->cols / 2 - 2; // 初始列位置（居中）
    g->block_dropped = new_block; // 更新当前下落方块
    g->next_block = game_random_block(); // 生成下一个方块类型
}
// 生成随机的方块类型 
type game_random_block(void){
    return (type)(rand() % NUM_TYPES); // 返回随机方块类型
}
// 检查并消除完整的行，返回消除的行数
int game_check_lines(game *g){
    int lines_cleared = 0; // 记录消除的行数
    int row = g->rows - 1;  // 从底部开始检查
    while (row >= 0) {
        bool full_line = true; // 假设当前行是满的
        for (int col = 0; col < g->cols; col++) {
            if (g->board[row][col] == EMPTY) {
                full_line = false; // 发现空单元格，当前行不是满的
                break;
            }
        }
        if (full_line == true) {
            lines_cleared++; // 发现一行满的
            // 消除当前行，所有上方行下移一行
            for (int r = row; r > 0; r--) {
                for (int c = 0; c < g->cols; c++) {
                    g->board[r][c] = g->board[r - 1][c]; 
                }
            }
            // 清空顶部行
            for (int c = 0; c < g->cols; c++) {
                g->board[0][c] = EMPTY;
            }
        } else {
            row--; // 仅在未消除行时才向上移动
        }
    }
    return lines_cleared; // 返回总共消除的行数
}
// 根据消除的行数更新分数和等级
void game_update_grade(game *g, int lines_cleared){
    // 更新分数
    if (lines_cleared >= 0 && lines_cleared <= 4) {
        g->points += SCORE_TABLE[lines_cleared];
    }
    // 更新等级
    g->lines_to_clear -= lines_cleared; // 减少升级所需行数
    while (g->lines_to_clear <= 0 && g->level < MAX_LEVEL) {
        g->level++; // 升级
        g->lines_to_clear += 10 + g->level * 5; // 更新升级所需行数
    }
}