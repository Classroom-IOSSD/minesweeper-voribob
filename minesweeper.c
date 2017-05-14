/**
 * @file minesweeper.c
 * @author JoonSeok Kim
 * @date 2017/04.13
 * @version 0.1
 * @brief minesweeper game
 *
 * @detail This is console-based Minesweeper game.
 *
 * */

#include <stdio.h>
#include <time.h>
#include "grid_table.h"
#include "minesweeper.h"

#define NEIGHBOR 8
#define DEFAULT_WIDTH 10
#define DEFAULT_HEIGHT 10

// mask values used for information
/// mask for flag: (1<<4) 0x10
const unsigned int FLAG_MASK = BIT(4);
/// mask for uncovered: (1<<5) 0x20
const unsigned int UNCOVERED_MASK = BIT(5);
/// mask for mine: (1<<6) 0x40
const unsigned int MINE_MASK = BIT(6);
/// mask for the number of mine
const unsigned int NUM_MINE_MASK = 0x0f;

// static variables
/// game options such as the size of game table and difficulty of the game
static GameConfiguration *config;
/// the total number of mines in the game
static size_t nMines;


bool has_mine(size_t column, size_t row) {
    return check_cell_mask(column, row, MINE_MASK);
}

void put_mine(size_t column, size_t row) {
    set_cell_mask(column, row, MINE_MASK);
}

bool is_uncovered(size_t column, size_t row) {
    return check_cell_mask(column, row, UNCOVERED_MASK);
}

void uncover_cell(size_t column, size_t row) {
    set_cell_mask(column, row, UNCOVERED_MASK);

}

bool is_flagged(size_t column, size_t row) {
    return check_cell_mask(column, row, FLAG_MASK);
}

void set_flag(size_t column, size_t row) {
    set_cell_mask(column, row, FLAG_MASK);
}

void unset_flag(size_t column, size_t row) {
    clear_cell_mask(column, row, FLAG_MASK);
}

size_t num_mines(size_t column, size_t row) {
    return get_mask(column, row, NUM_MINE_MASK);
}

bool is_blank_cell(size_t column, size_t row) {
    return num_mines(column, row) == 0 &&
           !is_uncovered(column, row) &&
           !has_mine(column, row);
}



/**
 * @brief uncover blank cells
 *
 * @detail
 * This is a recursive function which uncovers blank cells
 * while they are adjacent
 *
 * @param row
 * @param column
 *
*/
void uncover_blank_cell(int col, int row) {
    int rows[NEIGHBOR], columns[NEIGHBOR], i;

    uncover_cell(col, row); // uncover current cell

    // Get position of adjacent cells of current cell
    rows[0] = row - 1;
    columns[0] = col + 1;
    rows[1] = row;
    columns[1] = col + 1;
    rows[2] = row + 1;
    columns[2] = col + 1;
    rows[3] = row - 1;
    columns[3] = col;
    rows[4] = row + 1;
    columns[4] = col;
    rows[5] = row - 1;
    columns[5] = col - 1;
    rows[6] = row;
    columns[6] = col - 1;
    rows[7] = row + 1;
    columns[7] = col - 1;

    for(i = 0; i < NEIGHBOR; i++) {
        if( (rows[i] >= 0 && rows[i] < table_height()) &&
                (columns[i] >= 0 && columns[i] < table_width()) ) {	// to prevent negative index and out of bounds
            if(is_blank_cell(columns[i], rows[i]))
                uncover_blank_cell(columns[i], rows[i]);
            else if(!is_uncovered(columns[i], rows[i]) && !has_mine(columns[i], rows[i]))
                uncover_cell(columns[i], rows[i]);										// it is a cell with 1-8 number so we need to uncover
        }

    }

    return; // success!
}



void set_configuration(size_t width, size_t height, Difficulty difficulty) {
    if(config == NULL)
        config = (GameConfiguration*) malloc(sizeof(GameConfiguration));

    config->width = width;
    config->height = height;
    config->difficulty = difficulty;
}

void new_game() {
    if(config == NULL)
        set_configuration(DEFAULT_WIDTH, DEFAULT_HEIGHT, EASY);

    init_table(config->width, config->height);

    size_t num_mines = config->width * config->height;
    switch(config->difficulty) {
    case EASY:
        num_mines /= 10;
        break;
    case HARD:
        num_mines /= 7;
        break;
    case EXPERT:
        num_mines /= 5;
        break;
    }

    set_mine(num_mines);
}

void end_game() {
    free(config);
    config = NULL;
    free_table();
}

size_t get_num_mines() {
    return nMines;
}

void set_mine(size_t num_mines) {
    nMines = num_mines;
    int r, c, i, j, rows[NEIGHBOR], columns[NEIGHBOR];
    srand (time(NULL));						// random seed
    // set all cells to 0
    for(i = 0; i < nMines; i++) {
        /* initialize random seed: */

        r = rand() % table_height();
        c = rand() % table_width();

        // put mines
        if(!has_mine(c, r)) {
            put_mine(c, r);

            // Get position of adjacent cells of current cell
            rows[0] = r - 1;
            columns[0] = c + 1;
            rows[1] = r;
            columns[1] = c + 1;
            rows[2] = r + 1;
            columns[2] = c + 1;
            rows[3] = r - 1;
            columns[3] = c;
            rows[4] = r + 1;
            columns[4] = c;
            rows[5] = r - 1;
            columns[5] = c - 1;
            rows[6] = r;
            columns[6] = c - 1;
            rows[7] = r + 1;
            columns[7] = c - 1;


            for(j = 0; j < NEIGHBOR; j++) {
                if((rows[j] >= 0 && rows[j] < table_height()) &&
                        (columns[j] >= 0 && columns[j] < table_width())) {	// to prevent negative index and out of bounds
                    if(!has_mine(columns[j], rows[j]))														// to prevent remove mines
                        set_cell(columns[j], rows[j], get_cell(columns[j], rows[j])+1);									// sums 1 to each adjacent cell
                }
            }

        } else {							// to make sure that there are the properly number of mines in table
            i--;
            continue;
        }
    }


}



