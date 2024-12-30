require('dotenv').config();
const fs = require('fs');
const TradingView = require('@mathieuc/tradingview');


if (!process.env.SESSION || !process.env.SIGNATURE) {
    throw Error('Please set your sessionid and signature cookies');
}

const client = new TradingView.Client({
    token: process.env.SESSION,
    signature: process.env.SIGNATURE,
});

function declareMarket(market, timeframe, range) {
    const chart = new client.Session.Chart();
    chart.setMarket(market, {
        timeframe: timeframe,
        range: range,
        to: Date.now() / 1000, // Current time in seconds
    });
    return chart;
}

function fetchPriceData(chart) {
    return new Promise((resolve) => {
        chart.onUpdate(() => {
            resolve(chart.periods);
        });
    });
}

function getIndicData(indicatorId, market, timeframe, range) {
    const chart = declareMarket(market, timeframe, range);
    return new Promise((resolve) => {
        TradingView.getIndicator(indicatorId).then(async (indic) => {
            //console.log(`Loading '${indic.description}' study...`);
            const study = new chart.Study(indic);

            study.onUpdate(() => {
                resolve({ name: indic.description, periods: study.periods });
                chart.delete();
            });
        });
    });
}

function delay(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

async function getDataAndSaveToFile(market, timeframe, range, indicatorIds, filename) {
    const chart = declareMarket(market, timeframe, range);
    const priceData = await fetchPriceData(chart);
    const data = { prices: priceData };

    for (const id of indicatorIds) {
        const indicatorData = await getIndicData(id, market, timeframe, range);
        data[indicatorData.name] = indicatorData.periods;
        //await delay(1000); // 1 second delay
    }

    client.end();
    fs.writeFileSync(filename, JSON.stringify(data, null, 2));
    //console.log(`Data saved to ${filename}`);
}

const timeframe = process.argv[3] || '15'; // 15 minute timeframe
const market = process.argv[2] || 'BINANCE:BTCUSD';
const range = 2; // Get the last 5 candles
const indicatorIds = [
    'PUB;exeHlFteitBmMDhAn84FeDY0wecgKekk', // zero lag MACD
    'PUB;ae9bc503c1604345bb8b6968d92f466c', // ultimate RSI
    'PUB;68', // ichimoku
    'PUB;AQ5m8q9krLvMsvXtlbKesEd34hGNtuDo' // Analayse Technique Dynamique (ATD)
];
const filename = 'src/data/data.json';

getDataAndSaveToFile(market, timeframe, range, indicatorIds, filename);