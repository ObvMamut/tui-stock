Here is a draft of the README for your project:

---

# tui-stock

TUI Application that analyzes given stock/forex data and alerts the user when to open a position.

## Description

`tui-stock` is a terminal user interface (TUI) application designed to analyze stock and forex data. It provides various technical indicators to help users make informed trading decisions. The application reads data from YAML files and displays it in a user-friendly interface using `ncurses`.

## Features

- **Stock Data Analysis**: Parses and analyzes stock data from YAML files.
- **Technical Indicators**: Includes indicators such as MACD, RSI, Ichimoku, and more.
- **User Interface**: Provides a TUI for easy interaction and visualization of data.
- **Stock Management**: Allows users to manage their stock list and add new stocks.

## Installation

To install the dependencies, run:

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

Clone the repository:

```bash
git clone https://github.com/ObvMamut/tui-stock.git
cd tui-stock
```

Install the required Node.js packages:

```bash
npm install
```

## Usage

1. Initialize the application:

```bash
gcc src/main.c src/app.c src/data.c src/interact.c -o tui-stock -lncurses && ./tui-stock
```

2. Follow the on-screen prompts to log in, view the dashboard, or manage stocks.

## Project Structure

- `src/app.c`: Contains the main application logic and user interface.
- `src/data.c`: Handles data parsing and processing.
- `src/interact.c`: Provides functions for reading and writing YAML data.
- `src/main.c`: Entry point of the application.
- `src/api/`: Contains scripts for interacting with external APIs.
- `src/data/`: Directory for storing stock data and price indicators.

## Contributing

Contributions are welcome! Please create a pull request or open an issue to discuss your ideas.

## License

This project does not have a license specified.

## Author

- [ObvMamut](https://github.com/ObvMamut)

## Acknowledgments

Special thanks to all contributors who have helped improve this project.

---

Feel free to make any adjustments or additions as necessary.