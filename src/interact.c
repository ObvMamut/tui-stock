#include "../include/interact.h"
#include <ncurses.h>
#include <string.h>
#include <yaml.h>
#include <stdio.h>
#include <stdlib.h>

char** read_yml(const char* filename, int* count) {
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

    char** result = malloc(sizeof(char*) * 101); // Allocate memory for up to 100 strings + 1 for NULL terminator
    if (!result) {
        fputs("Failed to allocate memory!\n", stderr);
        yaml_parser_delete(&parser);
        fclose(file);
        return NULL;
    }

    *count = 0;

    int done = 0;
    while (!done) {
        if (!yaml_parser_parse(&parser, &event)) {
            fputs("Parser error\n", stderr);
            break;
        }

        switch (event.type) {
            case YAML_SCALAR_EVENT:
                if (*count < 100) {
                    result[*count] = strdup((char*)event.data.scalar.value);
                    if (!result[*count]) {
                        fputs("Failed to allocate memory for string!\n", stderr);
                        done = 1;
                    } else {
                        (*count)++;
                    }
                }
            break;
            case YAML_STREAM_END_EVENT:
                done = 1;
            break;
            default:
                break;
        }

        yaml_event_delete(&event);
    }

    result[*count] = NULL; // Null-terminate the array

    yaml_parser_delete(&parser);
    fclose(file);

    return result;
}
void write_yml(const char *filename, const char *new_data) {
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

void run_node_command(const char *market) {
    char command[256];
    snprintf(command, sizeof(command), "node src/api/get-price-and-indicator.js \"%s\"", market);
    int result = system(command);
    if (result == -1) {
        perror("Error executing command");

    }
}
