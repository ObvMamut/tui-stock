//
// Created by mamut on 12/27/24.
//
#include <stdio.h>
#include "../include/app.h"
#include <ncurses.h>
#include <string.h>
#include <yaml.h>
#include "../include/interact.h"
#include "../include/data.h"


int get_length(char **array) {
    int length = 0;
    while (array[length] != NULL) {
        length++;
    }
    return length;
}

void example_usage() {

    PriceData* prices;
    int price_count;
    MACDData* macd;
    int macd_count;
    RSIData* rsi;
    int rsi_count;
    IchimokuData* ichimoku;
    int ichimoku_count;
    ATDData* atd;
    int atd_count;

    parse_price_indicator_yml("src/data/price-indicator.yml", &prices, &price_count, &macd, &macd_count, &rsi, &rsi_count, &ichimoku, &ichimoku_count, &atd, &atd_count);


    // Print the close prices
    for (int i = 0; i < price_count; i++) {
        printf("Close Price %d: %f\n", i, prices[i].close);
    }


    // Use the parsed data...
}

void print_array(char** array) {
    int i = 0;
    while (array[i] != NULL) {
        printf("%s ", array[i]);
        free(array[i]); // Free each string after printing
        i++;
    }
    free(array); // Free the array itself
}

void init_app() {

    initscr();            // Initialize the window
    cbreak();             // Disable line buffering
    keypad(stdscr, TRUE); // Enable special keys to be interpreted by ncurses
    noecho();             // Do not display typed characters
}

void run_app() {


    example_usage();

    napms(2000);

    main_menu();
}

void cleanup_app() {
    endwin();             // End the window
}

void log_in() {
    clear(); // Clear the screen
    int height = 10, width = 40;
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width) / 2;

    WINDOW *login_win = newwin(height, width, start_y, start_x);
    box(login_win, 0, 0);
    mvwprintw(login_win, 1, (width - strlen("Enter Trading View Credentials")) / 2, "Log In");
    mvwprintw(login_win, 3, 2, "Username: ");
    mvwprintw(login_win, 5, 2, "Password: ");
    wrefresh(login_win);

    char username[30];
    char password[30];

    echo(); // Enable echoing of characters

    // Move cursor to the username input field
    mvwgetnstr(login_win, 3, 12, username, 29);
    // Move cursor to the password input field
    mvwgetnstr(login_win, 5, 12, password, 29);

    noecho(); // Disable echoing of characters

    // Clear the window and refresh
    wclear(login_win);
    wrefresh(login_win);
    delwin(login_win);

    // Display the entered username and password for debugging purposes
    clear();
}

void dashboard() {
    clear();
    refresh();

    int height = 55, width = 108;
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width) / 2;

    WINDOW *main_win = newwin(height, width, start_y, start_x);
    box(main_win, 0, 0);

    // create n subwindows

    // determine n based on number of stocks

    int count_n = 0;
    char** stocks = read_yml("src/data/data.yml", &count_n);
    int n = get_length(stocks);

    for (int i = 0; i < n; i++) {
        int sub_height = 3, sub_width = 106;
        int sub_start_y = start_y + 1 + i * 3;
        int sub_start_x = start_x + 1;

        WINDOW *sub_win = subwin(main_win, sub_height, sub_width, sub_start_y, sub_start_x);
        box(sub_win, 0, 0);

        // Add separators
        for (int j = 1; j <= 6; j++) {
            mvwprintw(sub_win, 1, j * (sub_width / 7), "|");
        }

        // 1 subwindow i the header of the table

        if (i == 0) {
            mvwprintw(sub_win, 1, 1, "Stock Name");
            mvwprintw(sub_win, 1, 1 + sub_width / 7, "Price");
            mvwprintw(sub_win, 1, 1 + 2 * sub_width / 7, "MACD");
            mvwprintw(sub_win, 1, 1 + 3 * sub_width / 7, "ATD");
            mvwprintw(sub_win, 1, 1 + 4 * sub_width / 7, "RSI");
            mvwprintw(sub_win, 1, 1 + 5 * sub_width / 7, "Ichimoku");
            mvwprintw(sub_win, 1, 1 + 6 * sub_width / 7, "Score");
        } else {

            // load in stock name, price + indicator

            // name

            char* stock = stocks[i];

            mvwprintw(sub_win, 1, 1, "%s", stock);

            // load for each stock new data

            // load data

            run_node_command(stock);

            // get data

            // print the prices and indicator data

            // int count_data = 0;  // Changed from int *count_data = 0
            // char **data_arr = read_yml("src/data/price-indicator.yml", &count_data);  // Pass address of count_data
            //
            // for (int i = 0; i < count_data; i++) {
            //     mvwprintw(sub_win, 1,1 + sub_width / 7, "%s", data_arr[i]);
            //     free(data_arr[i]);
            // }
            // free(data_arr);

            example_usage();

        }

        wrefresh(sub_win);
    }

    // add a info window at the bottom displaying the time frame used (15 min)

    WINDOW *info_win = subwin(main_win, 3, width, start_y + height - 3, start_x);
    box(info_win, 0, 0);

    // what time frame is loaded

    mvwprintw(info_win, 1, 1, "TIMEFRAME : 15 minute");


    wrefresh(main_win);



    int counter = 0;
    nodelay(stdscr, TRUE);  // Make getch() non-blocking

    while (1) {
        int ch = getch();
        if (ch > 0) {  // any key
            delwin(main_win);
            nodelay(stdscr, FALSE);  // Reset to blocking mode
            return;
        }

        wrefresh(main_win);

    }
}


void manage_stocks() {
    clear();
    refresh();

    int height = 6, width = 40;
    int start_y = (LINES - height - 40) / 2;
    int start_x = (COLS - width + 60) / 2;

    WINDOW *add_win = newwin(height, width, start_y, start_x);
    box(add_win, 0, 0);
    mvwprintw(add_win, 1, (width - strlen("Add New Stock")) / 2, "Add New Stock");
    mvwprintw(add_win, 3, 2, "Stock Symbol: ");
    wrefresh(add_win);

    // display the stocks from the database (src/data/stocks.json)

    int count = 0;
    char** stocks = read_yml("src/data/data.yml", &count);

    // get the length of stocks

    int length = get_length(stocks);

    int stock_height = (length + 3), stock_width = 60;
    int stock_start_y = (LINES - stock_height - 42 + length) / 2;
    int stock_start_x = (COLS - stock_width - 40) / 2;

    WINDOW *stocks_win = newwin(stock_height, stock_width, stock_start_y, stock_start_x);
    box(stocks_win, 0, 0);
    mvwprintw(stocks_win, 1, 2, "Loaded Stocks:");


    for (int i = 1; i < count; i++) {
        mvwprintw(stocks_win, i + 2, 2, "%s", stocks[i]);
        free(stocks[i]);
    }
    free(stocks);

    wrefresh(stocks_win);

    char stock_symbol[30];

    echo(); // Enable echoing of characters

    // Move cursor to the stock symbol input field
    int ch;
    int index = 0;
    while ((ch = mvwgetch(add_win, 3, 16 + index)) != '\n') {
        if (ch == 27) { // ESC key
            noecho();
            wclear(add_win);
            wrefresh(add_win);
            delwin(add_win);
            clear();
            refresh();
            return;
        }
        if (index < 29) {
            stock_symbol[index++] = ch;
            stock_symbol[index] = '\0';

        }
    }

    noecho(); // Disable echoing of characters

    // Add the stock to the database

    const char *new_data = stock_symbol;
    write_yml("src/data/data.yml", new_data);



}

void main_menu() {
    int choice;
    int highlight = 0;
    const char *choices[] = {
        "Log in",
        "Dashboard",
        "Manage Stocks",
        "Exit"
    };
    int n_choices = sizeof(choices) / sizeof(choices[0]);
    int x, y;

    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    while (1) {
        getmaxyx(stdscr, y, x); // Get the screen size
        int start_y = (y - n_choices) / 2; // Calculate the starting y position
        int start_x = (x - strlen(choices[0])) / 2; // Calculate the starting x position

        for (int i = 0; i < n_choices; ++i) {
            if (i == highlight)
                attron(A_REVERSE);
                mvprintw(start_y + i, start_x, "%s", choices[i]);            if (i == highlight)
                attroff(A_REVERSE);
        }
        choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight == 0) ? n_choices - 1 : highlight - 1;
            break;
            case KEY_DOWN:
                highlight = (highlight == n_choices - 1) ? 0 : highlight + 1;
            break;
            case 10: // Enter key
                clear();
            switch (highlight) {
                case 0:
                    log_in();
                break;
                case 1:
                    dashboard();
                break;
                case 2:
                    manage_stocks();
                break;
                case 3:
                    endwin();
                return;
            }
            break;
            default:
                break;
        }
        clear();
    }
}
