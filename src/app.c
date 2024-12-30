//
// Created by mamut on 12/27/24.
//
#include <stdio.h>
#include "app.h"
#include <ncurses.h>
#include <string.h>
#include <yaml.h>

// Function to read and print contents of a YAML file
char** read_yaml_file(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    yaml_parser_t parser;
    yaml_event_t event;

    if (!yaml_parser_initialize(&parser)) {
        fputs("Failed to initialize parser!\n", stderr);
        fclose(file);
        return NULL;
    }

    yaml_parser_set_input_file(&parser, file);

    char** result = malloc(sizeof(char*) * 100); // Allocate memory for up to 100 strings
    *count = 0;

    int done = 0;
    while (!done) {
        if (!yaml_parser_parse(&parser, &event)) {
            fputs("Parser error\n", stderr);
            break;
        }

        switch (event.type) {
            case YAML_SCALAR_EVENT:
                result[*count] = strdup((char*)event.data.scalar.value);
            (*count)++;
            break;
            case YAML_STREAM_END_EVENT:
                done = 1;
            break;
            default:
                break;
        }

        yaml_event_delete(&event);
    }

    yaml_parser_delete(&parser);
    fclose(file);

    return result;
}


void append_to_yaml(const char *filename, const char *new_data) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file for reading");
        return;
    }

    yaml_parser_t parser;
    yaml_document_t document;
    yaml_node_t *node;

    if (!yaml_parser_initialize(&parser)) {
        fputs("Failed to initialize parser!\n", stderr);
        fclose(file);
        return;
    }

    yaml_parser_set_input_file(&parser, file);

    if (!yaml_parser_load(&parser, &document)) {
        fputs("Failed to load YAML document!\n", stderr);
        yaml_parser_delete(&parser);
        fclose(file);
        return;
    }

    fclose(file);

    // Find the stocks sequence node
    yaml_node_t *stocks_node = NULL;
    for (int i = 1; i <= document.nodes.top - document.nodes.start; i++) {
        node = yaml_document_get_node(&document, i);
        if (node->type == YAML_MAPPING_NODE) {
            for (yaml_node_pair_t *pair = node->data.mapping.pairs.start;
                 pair < node->data.mapping.pairs.top; pair++) {
                yaml_node_t *key = yaml_document_get_node(&document, pair->key);
                if (key->type == YAML_SCALAR_NODE && strcmp((char *)key->data.scalar.value, "stocks") == 0) {
                    stocks_node = yaml_document_get_node(&document, pair->value);
                    break;
                }
            }
        }
        if (stocks_node) break;
    }

    if (!stocks_node || stocks_node->type != YAML_SEQUENCE_NODE) {
        fputs("Failed to find the stocks sequence!\n", stderr);
        yaml_document_delete(&document);
        yaml_parser_delete(&parser);
        return;
    }

    // Append the new data
    int new_scalar_index = yaml_document_add_scalar(
        &document, NULL, (yaml_char_t *)new_data, strlen(new_data), YAML_PLAIN_SCALAR_STYLE);

    // Reallocate the sequence items to add the new scalar
    int sequence_size = stocks_node->data.sequence.items.top - stocks_node->data.sequence.items.start;
    yaml_node_item_t *new_items = malloc((sequence_size + 2) * sizeof(yaml_node_item_t));
    memcpy(new_items, stocks_node->data.sequence.items.start, sequence_size * sizeof(yaml_node_item_t));
    new_items[sequence_size] = new_scalar_index;
    new_items[sequence_size + 1] = 0; // Null-terminate the items array

    free(stocks_node->data.sequence.items.start);
    stocks_node->data.sequence.items.start = new_items;
    stocks_node->data.sequence.items.top = new_items + sequence_size + 1;

    // Write the updated document back to the file
    file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for writing");
        yaml_document_delete(&document);
        yaml_parser_delete(&parser);
        return;
    }

    yaml_emitter_t emitter;
    if (!yaml_emitter_initialize(&emitter)) {
        fputs("Failed to initialize emitter!\n", stderr);
        fclose(file);
        yaml_document_delete(&document);
        yaml_parser_delete(&parser);
        return;
    }

    yaml_emitter_set_output_file(&emitter, file);
    yaml_emitter_dump(&emitter, &document);

    yaml_emitter_delete(&emitter);
    yaml_document_delete(&document);
    yaml_parser_delete(&parser);
    fclose(file);
}
void init_app() {
    initscr();            // Initialize the window
    cbreak();             // Disable line buffering
    keypad(stdscr, TRUE); // Enable special keys to be interpreted by ncurses
    noecho();             // Do not display typed characters
}

void run_app() {
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

    int height = 55, width = 86;
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width) / 2;

    WINDOW *main_win = newwin(height, width, start_y, start_x);
    box(main_win, 0, 0);

    // create 5 subwindows

    for (int i = 0; i < 17; i++) {
        int sub_height = 3, sub_width = 84;
        int sub_start_y = start_y + 1 + i * 3;
        int sub_start_x = start_x + 1;

        WINDOW *sub_win = subwin(main_win, sub_height, sub_width, sub_start_y, sub_start_x);
        box(sub_win, 0, 0);

        // Add separators
        for (int j = 1; j <= 4; j++) {
            mvwprintw(sub_win, 1, j * (sub_width / 5), "|");
        }

        // 1 subwindow i the header of the table

        if (i == 0) {
            mvwprintw(sub_win, 1, 1, "Stock Name");
            mvwprintw(sub_win, 1, 1 + sub_width / 5, "Price");
            mvwprintw(sub_win, 1, 1 + 2 * sub_width / 5, "Change");
            mvwprintw(sub_win, 1, 1 + 3 * sub_width / 5, "Volume");
            mvwprintw(sub_win, 1, 1 + 4 * sub_width / 5, "Market Cap");
        } else {
            mvwprintw(sub_win, 1, 1, "Subwindow %d", i + 1);
        }

        wrefresh(sub_win);
    }


    // add a info window at the bottom displaying the time frame used (15 min)

    WINDOW *info_win = subwin(main_win, 3, width, start_y + height - 3, start_x);
    box(info_win, 0, 0);

    // what time frame is loaded

    mvwprintw(info_win, 1, 1, "TIMEFRAME : 15 minute");

    int counter = 0;
    nodelay(stdscr, TRUE);  // Make getch() non-blocking

    while (1) {
        int ch = getch();
        if (ch > 0) {  // any key
            delwin(main_win);
            nodelay(stdscr, FALSE);  // Reset to blocking mode
            return;
        }

        // Update counter and display
        //mvwprintw(main_win, 1, 1, "Counter: %d", counter++);
        //box(main_win, 0, 0);  // Redraw the box
        wrefresh(main_win);

        napms(1000);  // Sleep for 2 seconds
    }
}

int get_length(char **array) {
    int length = 0;
    while (array[length] != NULL) {
        length++;
    }
    return length;
}

void add_stock() {
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
    char** stocks = read_yaml_file("src/data/data.yml", &count);

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
    append_to_yaml("src/data/data.yml", new_data);



}

void main_menu() {
    int choice;
    int highlight = 0;
    const char *choices[] = {
        "Log in",
        "Dashboard",
        "Add stock",
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
                    add_stock();
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
