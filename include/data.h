//
// Created by mamut on 12/31/24.
//

#ifndef DATA_H
#define DATA_H


typedef struct {
    long time;
    double open;
    double close;
    double max;
    double min;
    double volume;
} PriceData;

typedef struct {
    long time;
    double Positive_delta;
    double Negative_delta;
    double MACD_line;
    double Signal;
    double EMA_on_MACD_line;
    double Dots;
} MACDData;

typedef struct {
    long time;
    double Ultimate_RSI;
    double Signal_Line;
} RSIData;

typedef struct {
    long time;
    double Tenkan;
    double Kijun;
    double Chikou;
    double SenkouA;
    double SenkouB;
} IchimokuData;

typedef struct {
    long time;
    double Plot;
    double Plot_2;
    double Fast_MM;
    double Slow_MM;
    double Up_Trending_SAR_2;
    double Up_Trending_SAR;
} ATDData;

void parse_price_indicator_yml(const char* filename, PriceData** prices, int* price_count, MACDData** macd, int* macd_count, RSIData** rsi, int* rsi_count, IchimokuData** ichimoku, int* ichimoku_count, ATDData** atd, int* atd_count);

#endif // DATA_H

