//
// Created by mamut on 12/31/24.
//

#include "../include/data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yaml.h>


void parse_price_indicator_yml(const char* filename, PriceData** prices, int* price_count, MACDData** macd, int* macd_count, RSIData** rsi, int* rsi_count, IchimokuData** ichimoku, int* ichimoku_count, ATDData** atd, int* atd_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    yaml_parser_t parser;
    yaml_event_t event;

    if (!yaml_parser_initialize(&parser)) {
        fputs("Failed to initialize parser!\n", stderr);
        fclose(file);
        return;
    }

    yaml_parser_set_input_file(&parser, file);

    *prices = malloc(sizeof(PriceData) * 100);
    *macd = malloc(sizeof(MACDData) * 100);
    *rsi = malloc(sizeof(RSIData) * 100);
    *ichimoku = malloc(sizeof(IchimokuData) * 100);
    *atd = malloc(sizeof(ATDData) * 100);

    *price_count = 0;
    *macd_count = 0;
    *rsi_count = 0;
    *ichimoku_count = 0;
    *atd_count = 0;

    int done = 0;
    int current_section = 0; // 0: none, 1: prices, 2: macd, 3: rsi, 4: ichimoku, 5: atd

    while (!done) {
        if (!yaml_parser_parse(&parser, &event)) {
            fputs("Parser error\n", stderr);
            break;
        }

        switch (event.type) {
            case YAML_MAPPING_START_EVENT:
                // Handle mapping start
                break;
            case YAML_MAPPING_END_EVENT:
                // Handle mapping end
                break;
            case YAML_SEQUENCE_START_EVENT:
                // Handle sequence start
                break;
            case YAML_SEQUENCE_END_EVENT:
                // Handle sequence end
                break;
            case YAML_SCALAR_EVENT:
                if (strcmp((char*)event.data.scalar.value, "prices") == 0) {
                    current_section = 1;
                } else if (strcmp((char*)event.data.scalar.value, "Zero Lag MACD Enhanced - Version 1.2") == 0) {
                    current_section = 2;
                } else if (strcmp((char*)event.data.scalar.value, "Ultimate RSI [LuxAlgo]") == 0) {
                    current_section = 3;
                } else if (strcmp((char*)event.data.scalar.value, "Ichimoku2c") == 0) {
                    current_section = 4;
                } else if (strcmp((char*)event.data.scalar.value, "Analyse Technique Dynamique - All In One - v1.0") == 0) {
                    current_section = 5;
                } else {
                    // Handle scalar values based on the current section
                    switch (current_section) {
                        case 1:
                            // Parse PriceData
                            if (strcmp((char*)event.data.scalar.value, "time") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*prices)[*price_count].time = atol((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "open") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*prices)[*price_count].open = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "close") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*prices)[*price_count].close = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "max") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*prices)[*price_count].max = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "min") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*prices)[*price_count].min = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "volume") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*prices)[*price_count].volume = atof((char*)event.data.scalar.value);
                                (*price_count)++;
                            }
                            break;
                        case 2:
                            // Parse MACDData
                            if (strcmp((char*)event.data.scalar.value, "$time") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*macd)[*macd_count].time = atol((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Positive_delta") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*macd)[*macd_count].Positive_delta = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Negative_delta") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*macd)[*macd_count].Negative_delta = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "MACD_line") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*macd)[*macd_count].MACD_line = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Signal") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*macd)[*macd_count].Signal = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "EMA_on_MACD_line") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*macd)[*macd_count].EMA_on_MACD_line = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Dots") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*macd)[*macd_count].Dots = atof((char*)event.data.scalar.value);
                                (*macd_count)++;
                            }
                            break;
                        case 3:
                            // Parse RSIData
                            if (strcmp((char*)event.data.scalar.value, "$time") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*rsi)[*rsi_count].time = atol((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Ultimate_RSI") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*rsi)[*rsi_count].Ultimate_RSI = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Signal_Line") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*rsi)[*rsi_count].Signal_Line = atof((char*)event.data.scalar.value);
                                (*rsi_count)++;
                            }
                            break;
                        case 4:
                            // Parse IchimokuData
                            if (strcmp((char*)event.data.scalar.value, "$time") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*ichimoku)[*ichimoku_count].time = atol((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Tenkan") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*ichimoku)[*ichimoku_count].Tenkan = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Kijun") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*ichimoku)[*ichimoku_count].Kijun = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Chikou") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*ichimoku)[*ichimoku_count].Chikou = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "SenkouA") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*ichimoku)[*ichimoku_count].SenkouA = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "SenkouB") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*ichimoku)[*ichimoku_count].SenkouB = atof((char*)event.data.scalar.value);
                                (*ichimoku_count)++;
                            }
                            break;
                        case 5:
                            // Parse ATDData
                            if (strcmp((char*)event.data.scalar.value, "$time") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*atd)[*atd_count].time = atol((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Plot") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*atd)[*atd_count].Plot = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Plot_2") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*atd)[*atd_count].Plot_2 = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Fast_MM") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*atd)[*atd_count].Fast_MM = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Slow_MM") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*atd)[*atd_count].Slow_MM = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Up_Trending_SAR_2") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*atd)[*atd_count].Up_Trending_SAR_2 = atof((char*)event.data.scalar.value);
                            } else if (strcmp((char*)event.data.scalar.value, "Up_Trending_SAR") == 0) {
                                yaml_parser_parse(&parser, &event);
                                (*atd)[*atd_count].Up_Trending_SAR = atof((char*)event.data.scalar.value);
                                (*atd_count)++;
                            }
                            break;
                        default:
                            break;
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

    yaml_parser_delete(&parser);
    fclose(file);
}