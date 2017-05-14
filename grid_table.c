/************************************************************************************//**
 *  @file       grid_table.c
 *
 *  @brief      Grid table for general purpose
 *
 *  @date       2017-04-27 18:04
 *
 ***************************************************************************************/


#include "grid_table.h"



/// grid table
static unsigned int **grid_table;
/// width of grid_table
static size_t _width;
/// height of grid_table
static size_t _height;


/**
 * @brief initialize grid table with width and height
 *
 * @param width of grid table
 * @param height of grid table
 * @return return true if memory allocation succeed
 * */
bool init_table(size_t width, size_t height) {
    // check if grid_table exist already
    if(grid_table != NULL)
        free_table();

    _width = width;
    _height = height;
    // allocate memory for rows (array of grid table)
    grid_table = (unsigned int **) malloc(sizeof(unsigned int**) * height);
    // for each row, allocate memory for columns
    for(int i=0; i<height; i++) {
        // initialize each cell with zero
        grid_table[i] = (unsigned int *) malloc(sizeof(unsigned int*) * width);
        for(int j=0; j<width; j++)
            grid_table[i][j] = 0;
    }
    return true;
}


/**
 * @brief free all memory of grid_table
 *
 * */
bool free_table() {
    if(grid_table != NULL) {
        for(int i=0; i<_height; i++)
            free(grid_table[i]);

        free(grid_table);
        grid_table = NULL;
    } else
        return false;

    return true;
}

size_t table_height() {
    return _height;
}

size_t table_width() {
    return _width;
}

static inline bool is_within_bound(size_t column, size_t row) {
    return column < _height && row < _width;
}

void set_cell(size_t column, size_t row, unsigned int value) {
//    if(!is_within_bound(column, row))
//        return false;

    grid_table[row][column] = value;
//    return true;
}


//bool get_cell(size_t column, size_t row, unsigned int *value) {
//    if(!is_within_bound(column, row))
//        return false;
//    *value = grid_table[row][column];
//    return true;
//}

unsigned int get_cell(size_t column, size_t row) {
    return grid_table[row][column];
}

void set_cell_mask(size_t column, size_t row, unsigned int mask) {
    BITMASK_SET(grid_table[row][column], mask);
}

void clear_cell_mask(size_t column, size_t row, unsigned int mask) {
    BITMASK_CLEAR(grid_table[row][column], mask);
}


bool check_cell_mask(size_t column, size_t row, unsigned int mask) {
    return BITMASK_CHECK(grid_table[row][column], mask);
}

unsigned int get_mask(size_t column, size_t row, unsigned int mask) {
    return grid_table[row][column] & mask;
}


