#ifndef TETRIS_H
#define TETRIS_H  // 标准的头文件保护
#include <stdio.h>
#include <stdbool.h>

/*常量定义*/
#define TETRIS 4           //每个俄罗斯方块由4个小方块组成
#define NUM_TYPES 7        //有7种不同形状的俄罗斯方块
#define NUM_ORIENTATIONS 4 //每个方块有4种可能的旋转方向
#define MAX_LEVEL 19       //游戏最高等级


/*枚举定义*/
typedef enum {
    EMPTY, CELL_I, CELL_J, CELL_L, CELL_O, CELL_S, CELL_T, CELL_Z
} cell;
/*cell是俄罗斯方块中单元格，定义每个单元格的可能状态*/
typedef enum {
    TET_I, TET_J, TET_L, TET_O, TET_S, TET_T, TET_Z
} type;
/*定义7种俄罗斯方块的类型，不包括旋转后的形状*/
typedef enum {
    MOV_LEFT, MOV_RIGHT, MOV_CLOCK, MOV_DROP, MOV_STORE, MOV_NONE //
} move;
/*定义所有可能的玩家操作*/

/*结构体定义*/
typedef struct {
    int row;   //行
    int col;   //列
} location;
/*位置，行坐标和列坐标*/
typedef struct {
    int typ;   //方块类型type(TET_I, TET_J等)
    int ori;   //旋转方向 (0-3)
    location loc;
} tetris;
/*表示一个俄罗斯方块的完整信息*/
typedef struct {
    /*游戏板信息*/
    int rows;  // 游戏板行数
    int cols;  // 游戏板列数
    cell **board;  // 指向二维数组的指针，表示游戏板
    /*当前方块信息*/
    tetris *block_dropped;  // 指向当前方块的指针
    tetris *block_stored;    // 指向保留方块的指针
    bool store_used;   // 标志，表示当前下落周期内是否已使用保留功能
    /*下一个方块*/
    type next_block;    // 存储下一个方块
    /*游戏状态信息*/
    int level;             // 当前等级
    int points;            // 当前得分
    int lines_to_clear;   // 升级所需消除的行数
    int tick_timer;      // 用于控制下落速度的计时器
} game;
/*核心游戏结构体*/

/* 声明方块形状数据（三维数组方式）*/
extern location shape[NUM_TYPES][NUM_ORIENTATIONS][TETRIS];  // 每种类型和旋转方向对应的4个小方块位置
/* 声明重力速度表 */
extern int GRAVITY_LEVEL[MAX_LEVEL + 1];  // 每个等级对应不同的下落速度
/* 声明消行得分表 */
extern int SCORE_TABLE[5];  // 0-4行对应的基础分数


/* 游戏初始化与销毁(对应logic.c) */
game* game_create(int rows, int cols);  // 创建并初始化游戏对象
void game_init(game *g, int rows, int cols);  // 初始化游戏对象
void game_destroy(game *g);  // 销毁游戏对象

/* 游戏逻辑(对应logic.c) */
bool game_tick(game *g, move mov);  // 处理游戏的每个tick，返回游戏是否继续
bool game_is_over(const game *g);  // 检查游戏是否结束

/* 方块操作(对应block.c) */
void game_move_left(game *g);  // 向左移动方块
void game_move_right(game *g);  // 向右移动方块
void game_clock_roll(game *g);  // 顺时针旋转方块
void game_drop(game *g);  // 快速下落方块(硬降落)
void game_store(game *g);  // 暂存/交换方块

/* 游戏板操作(对应board.c) */
cell game_get_cell_status(const game *g, int row, int col);  // 获取指定位置的单元格状态
void game_set_cell_status(game *g, int row, int col, cell value);  // 设置指定位置的单元格状态
bool game_is_valid_position(const game *g, int row, int col);  // 检查指定位置是否在棋盘范围内
void display_board(game *g);  // 显示游戏板

/* 辅助函数(对应logic.c) */
void game_create_new_block(game *g);  // 生成新的下落方块
type game_random_block(void);  // 生成随机的方块类型
int game_check_lines(game *g);  // 检查并消除完整的行，返回消除的行数
void game_update_grade(game *g, int lines_cleared);  // 根据消除的行数更新分数和等级

#endif