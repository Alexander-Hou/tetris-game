#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"

/*检查方块在指定偏移位置是否合法，row_offset和col_offset表示相对于当前方块位置的偏移*/
static bool cell_is_valid_position(const game *g, const tetris *block, int row_offset, int col_offset) {
    for (int i = 0; i < TETRIS; i++) {
        location pos = shape[block->typ][block->ori][i];  // 获取方块的每个小方块位置
        int new_row = block->loc.row + pos.row + row_offset; // 计算新的行位置
        int new_col = block->loc.col + pos.col + col_offset; // 计算新的列位置
        // 检查是否在边界内
        if (!game_is_valid_position(g, new_row, new_col)) {
            return false;
        }
        // 检查该位置是否为空
        if (game_get_cell_status(g, new_row, new_col) != EMPTY) {
            return false;
        }
    }
    return true;
}
// 向左移动方块
void game_move_left(game *g){
    if (cell_is_valid_position(g, g->block_dropped, 0, -1)) {
        g->block_dropped->loc.col -= 1;
    }
}
// 向右移动方块  
void game_move_right(game *g){
    if (cell_is_valid_position(g, g->block_dropped, 0, 1)) {
        g->block_dropped->loc.col += 1;
    }
}
// 顺时针旋转方块
void game_clock_roll(game *g){
    int old_ori = g->block_dropped->ori;  // 保存旧的方向
    int new_ori = (g->block_dropped->ori + 1) % NUM_ORIENTATIONS;  // 计算新的方向
    g->block_dropped->ori = new_ori;
    // 检查新方向是否合法,简单的墙踢实现
    if(!cell_is_valid_position(g, g->block_dropped, 0, 0)) {
        // 尝试左移
        if (cell_is_valid_position(g, g->block_dropped, 0, -1)) {
            g->block_dropped->loc.col -= 1;
        }
        // 尝试右移
        else if (cell_is_valid_position(g, g->block_dropped, 0, 1)) {
            g->block_dropped->loc.col += 1;
        }
        // 尝试向上（特殊情况）
        else if (cell_is_valid_position(g, g->block_dropped, -1, 0)) {
            g->block_dropped->loc.row--;
        }
        // 恢复原方向
        else {
            g->block_dropped->ori = old_ori;
        }
    }
}

// 快速下落方块(硬降落)
void game_drop(game *g) {
    while (cell_is_valid_position(g, g->block_dropped, 1, 0)) {
        g->block_dropped->loc.row += 1;
    }
}
// 暂存/交换方块
void game_store(game *g){
    if (g->store_used == true) {
        return; // 已经使用过保留功能，直接返回
    }
    if (g->block_stored==NULL) {
        g->block_stored = (tetris*)malloc(sizeof(tetris));  // 分配内存
        if (g->block_stored == NULL) {
            return; // 内存分配失败，直接返回
        }
        *g->block_stored = *g->block_dropped;
        game_create_new_block(g);
    } else if (g->block_stored->typ == -1) {
        *g->block_stored = *g->block_dropped;
        game_create_new_block(g);
    } else {
        // 交换当前方块和保留方块
        tetris temp = *g->block_dropped;
        *g->block_dropped = *g->block_stored;
        *g->block_stored = temp;
        // 重置当前方块位置和方向
        g->block_dropped->loc.row = 0;
        g->block_dropped->loc.col = g->cols / 2 - 2;
        g->block_dropped->ori = 0;
    }
    g->store_used = true; // 标记保留功能已使用
}