/*************************************************************************************//**
 *  @file       minesweeper.h
 *
 *  @brief      Brief description of minesweeper.h
 *
 *  @date       2017-04-28 15:58
 *
 **************************************************************************************/


#ifndef MINESWEEPER_H
#define MINESWEEPER_H


typedef enum {
    EASY = 0,
    HARD = 1,
    EXPERT = 2,
} Difficulty;

typedef struct {
    size_t width;
    size_t height;
    Difficulty difficulty;
} GameConfiguration;

bool has_mine(size_t column, size_t row);
void put_mine(size_t column, size_t row);
bool is_uncovered(size_t column, size_t row);
void uncover_cell(size_t column, size_t row);
bool is_flagged(size_t column, size_t row);
void set_flag(size_t column, size_t row);
void unset_flag(size_t column, size_t row);
size_t num_mines(size_t column, size_t row);
bool is_blank_cell(size_t column, size_t row);
void uncover_blank_cell(int col, int row);
void set_mine(size_t nMines);
void set_configuration(size_t width, size_t height, Difficulty difficulty);
size_t get_num_mines();
void new_game();

#endif
