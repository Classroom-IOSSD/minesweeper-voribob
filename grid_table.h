/*************************************************************************************//**
 *  @file       grid_table.h
 *
 *  @brief      Brief description of grid_table.h
 *
 *  @date       2017-04-27 18:11
 *
 **************************************************************************************/


#ifndef GRID_TABLE_H
#define GRID_TABLE_H

#include <stdlib.h>
#include <stdbool.h>
#include "bitmask.h"


#ifdef __cplusplus
extern "C" {
#endif


bool init_table(size_t, size_t);
bool free_table(void);
size_t table_width(void);
size_t table_height(void);
void set_cell(size_t column, size_t row, unsigned int value);
unsigned int get_cell(size_t column, size_t row);
void set_cell_mask(size_t column, size_t row, unsigned int mask);
void clear_cell_mask(size_t column, size_t row, unsigned int mask);
bool check_cell_mask(size_t column, size_t row, unsigned int mask);
unsigned int get_mask(size_t column, size_t row, unsigned int mask);


#ifdef __cplusplus
}
#endif


#endif /* !GRID_TABLE_H */

