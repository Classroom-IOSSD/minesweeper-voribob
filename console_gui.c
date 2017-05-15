/************************************************************************************//**
 *  @file       console_gui.c
 *
 *  @brief      console game gui
 *
 *  @date       2017-04-28 09:07
 *
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <string.h>
#include "conio.h"
#include "console_gui.h"
#include "grid_table.h"
#include "minesweeper.h"

// background color
#define KNRM  "\x1B[0m"
#define BBLK  "\x1B[40m"
#define BRED  "\x1B[41m"
#define BGRN  "\x1B[42m"
#define BYEL  "\x1B[43m"
#define BBLU  "\x1B[44m"
#define BMAG  "\x1B[45m"
#define BCYN  "\x1B[46m"
#define BWHT  "\x1B[47m"

// text color
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

// constants
enum {
    // limit of size of game table
    MAX_WIDTH = 40,
    MAX_HEIGHT = 40,
    MIN_WIDTH = 5,
    MIN_HEIGHT = 5,
    // for print
    CELL_SIZE = 11,
    BUFFER_SIZE = MAX_WIDTH * MAX_HEIGHT * CELL_SIZE + 1024,
};

static void menu_mode();
static void flag_mode();
static void check_mode();
static void end_of_game();
static void print_table();
static void init();
static void setting_mode();
static void overwrite_cell(int, int, char);
static int draw_cell(int, int, char);
static void clear_screen();

typedef enum {
    NEW_GAME = 0,
    SETTING,
    MENU,
    FLAG,
    CHECK,
    GAME_OVER,
    EXIT,
} GAME_STATUS;

static GAME_STATUS game_mode = NEW_GAME;
static size_t nMines;
static size_t found_num_mines;
static size_t num_flags;
static int cursor_x;
static int cursor_y;
static char buffer[BUFFER_SIZE];
static _Bool dirty;


void init() {
    // setting cursor
    cursor_x = 0;
    cursor_y = 0;

    nMines = get_num_mines();
    found_num_mines = nMines;
    num_flags = 0;
    game_mode = MENU;
    dirty = true;
}

void start_menu() {
    do {
        switch(game_mode) {
        case NEW_GAME:
            new_game();
            init();
            break;
        case SETTING:
            setting_mode();
            break;
        case MENU:
            menu_mode();
            break;
        case FLAG:
            flag_mode();
            break;
        case CHECK:
            check_mode();
            break;
        case GAME_OVER:
            end_of_game();
            break;
        default:
            break;
        }
        dirty = true;
    } while (game_mode != EXIT);

}

void setting_mode() {
    int width, height, difficulty;
    clear_screen();
    do {
        puts("Please input width (5-40):");
        scanf("%d", &width);
    } while(width < 5 || width > MAX_WIDTH);

    do {
        puts("Please input height (5-40):");
        scanf("%d", &height);
    } while(height < 5 || height > MAX_WIDTH);


    do {
        puts("Choose difficulty easy(0), hard(1), expert(2):");
        scanf("%d", &difficulty);
    } while(difficulty < 0 || difficulty > 2);

    // clear buffer
    getch();
    set_configuration(width, height, difficulty);

    game_mode = NEW_GAME;
}


void menu_mode() {
    char ch;
    print_table();

    ch = getch();
    switch (ch) {

    case 's':
    case 'S':
        game_mode = SETTING;
        break;
    // flag
    case 'f':
    case 'F':
        game_mode = FLAG;
        break;
    case 'c':
    case 'C':
        game_mode = CHECK;
        break;
    case 'n':
    case 'N':
        game_mode = NEW_GAME;
        break;
    // exit
    case 'q':
    case 'Q':
        game_mode = GAME_OVER;
        break;
    default:
        break;
    }

}

void flag_mode() {

    char ch;
    do {
        print_table();
        ch = getch();
        // arrow direction
        if(ch == '8') {
            // up
            cursor_y--;
            cursor_y = (table_height() + cursor_y) % table_height();
        } else if(ch == '2') {
            // down
            cursor_y++;
            cursor_y %= table_height();
        } else if(ch == '4') {
            cursor_x--;
            cursor_x = (table_width() + cursor_x) % table_width();
        } else if(ch == '6') {
            cursor_x++;
            cursor_x %= table_width();
        } else if(ch == 'c' || ch == 'C') {
            game_mode = CHECK;
        } else if (ch == 'q' || ch == 'Q') {
            game_mode = MENU;
        } else if(ch == '\n') {

            if (!is_uncovered(cursor_x, cursor_y)) {
                if(is_flagged(cursor_x, cursor_y)) {
                    unset_flag(cursor_x, cursor_y);
                    num_flags--;
                    if(has_mine(cursor_x, cursor_y))
                        found_num_mines += 1;

                } else {
                    set_flag(cursor_x, cursor_y);
                    num_flags++;
                    if(has_mine(cursor_x, cursor_y))
                        found_num_mines -= 1;
                }
            }

            if(num_flags == nMines && found_num_mines == 0) {
                game_mode = GAME_OVER;
            }

        }
    } while (game_mode == FLAG);

}

void check_mode() {
    char ch;

    do {
        print_table();
        ch = getch();

        // arrow direction
        if(ch == '8') {
            // up
            cursor_y--;
            cursor_y = (table_height() + cursor_y) % table_height();
        } else if(ch == '2') {
            // down
            cursor_y++;
            cursor_y %= table_height();
        } else if(ch == '4') {
            cursor_x--;
            cursor_x = (table_width() + cursor_x) % table_width();
        } else if(ch == '6') {
            cursor_x++;
            cursor_x %= table_width();
        } else if(ch == 'f' || ch == 'F') {
            game_mode = FLAG;
        } else if (ch == 'q' || ch == 'Q') {
            game_mode = MENU;
        } else if(ch == '\n') {
            if(is_blank_cell(cursor_x, cursor_y))						// blank case
                uncover_blank_cell(cursor_x, cursor_y);
            else if(has_mine(cursor_x, cursor_y))				// mine case
                game_mode = GAME_OVER;
            else if(!is_uncovered(cursor_x, cursor_y))	// number case (the next cell is a mine)
                uncover_cell(cursor_x, cursor_y);

            dirty = true;
        }
    } while (game_mode == CHECK);


}


void end_of_game() {
    char ch;
    print_table();
    printf("\nGAME OVER\n");

    if(found_num_mines == 0)
        printf("you won!!!!\n");
    else
        printf("BOOM! you LOOSE!\n");

    do {
        printf("Are you sure to exit? (y or n)? ");
        ch = getch();
        putchar('\n');
        if(ch == 'y' || ch == 'Y') {
            printf("See you next time!\n");
            game_mode = EXIT;
            break;
        } else if(ch == 'n' || ch == 'N') {
            game_mode = NEW_GAME;
            break;
        }
        printf("Please answer y or n\n");
    } while(game_mode == GAME_OVER);

}

void clear_screen() {
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

/**
 * @brief draw cell
 * */
int draw_cell(int x, int y, char cursor) {
    int offset = CELL_SIZE * (table_width() + 1) * y + CELL_SIZE * x;
    int length = 0;
    // number of mines near by the cell
    int n = num_mines(x, y);
    if(cursor) {
        length += sprintf(&buffer[offset], "|%s%c%s", BMAG, cursor, KNRM);
    } else if(is_flagged(x, y))
        length += sprintf(&buffer[offset], "|%sF%s", KGRN, KNRM);
    else if(!is_uncovered(x, y))
        length += sprintf(&buffer[offset], "|%sX%s", BBLK, KNRM);
    else if(n == 0) // clear area
        length += sprintf(&buffer[offset], "|%s%d%s", KCYN, n, KNRM);
    else if(n == 1) //
        length += sprintf(&buffer[offset], "|%s%d%s", KYEL, n, KNRM);
    else if(n > 1)
        length += sprintf(&buffer[offset], "|%s%d%s", KRED, n, KNRM);

    buffer[offset+CELL_SIZE] = '|';
    return length;
}

void overwrite_cell(int x, int y, char cursor) {
    int n = num_mines(x, y);
    // move cursor to 
    printf("\033[%d;%dH", y+1, 2*x+1);
    if(cursor) {
        printf("|%s%c%s", BMAG, cursor, KNRM);
    } else if(is_flagged(x, y))
        printf("|%sF%s", KGRN, KNRM);
    else if(!is_uncovered(x, y))
        printf("|%sX%s", BBLK, KNRM);
    else if(n == 0) // clear area
        printf("|%s%d%s", KCYN, n, KNRM);
    else if(n == 1) //
        printf("|%s%d%s", KYEL, n, KNRM);
    else if(n > 1)
        printf("|%s%d%s", KRED, n, KNRM);

    fflush(stdout);
}

void print_table() {
    int offset = 0;
    static int preX, preY;
    if(dirty) {
        // if the screen needs to be cleaned up

        // write all data into a buffer first
        // and then print the buffer

        int i, j;
        for(i = 0; i < table_height(); i++) {
            for(j = 0; j < table_width(); j++) {
                if(cursor_x == j && cursor_y == i) {
                    if(game_mode == FLAG) {
                        offset += draw_cell(j, i, 'F');
                        continue;
                    } else if(game_mode == CHECK) {
                        offset += draw_cell(j, i, 'C');
                        continue;
                    } // end if gmae_mode
                } // end if cursor
                offset += draw_cell(j, i, 0);
            } // end for j
            offset += sprintf(&buffer[offset], "|%s\n%s", BBLK, KNRM);
        } // end for i

        // menu
        offset += sprintf(&buffer[offset],
            "cell values: 'X' unknown, '%s0%s' no mines close, '1-8' number of near mines, '%sF%s' flag in cell\n", KCYN,KNRM,KGRN,KNRM);
        if(game_mode == MENU) {
            offset += sprintf(&buffer[offset], 
                "f (put/remove Flag in cell), c (Check cell), s (Setting), n (New game), q (Exit game): ");
        } else if(game_mode == FLAG) {
            offset += sprintf(&buffer[offset],
               "Enter (select to put/remove Flag in cell), q (Exit selection): ");
        } else if(game_mode == CHECK) {
            offset += sprintf(&buffer[offset],
                "Enter (select to check cell), q (Exit selection): ");
        }

        // screen clear
        clear_screen();
        fprintf(stdout, "%s", buffer);
        fflush(stdout);
        dirty = false;
    } else {
        // if the screen doesn't need to be cleaned up
        // update previous cursor and current cursor
        overwrite_cell(preX, preY, 0);

        if(game_mode == FLAG)
            overwrite_cell(cursor_x, cursor_y, 'F');
        else if (game_mode == CHECK)
            overwrite_cell(cursor_x, cursor_y, 'C');
    }
    preX = cursor_x;
    preY = cursor_y;
}

