#ifndef TETRIS_H
#define TETRIS_H  //标准的头文件保护
#include <stdio.h>
#include <stdbool.h>

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
    MOV_LEFT, MOV_RIGHT, MOV_CLOCK, MOV_DROP, MOV_HOLD, MOV_NONE //
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
    bool hold_used;   // 标志，表示当前下落周期内是否已使用保留功能
    /*下一个方块队列*/
    type next_block;    // 存储下一个方块
    /*游戏状态信息*/
    int level;             // 当前等级
    int points;            // 当前得分
    int lines_remaining;   // 升级所需消除的行数
} game;
/*核心游戏结构体*/


#endif