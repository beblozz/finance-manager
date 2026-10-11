# Finance Manager

A console app for tracking personal income and expenses, written in C++17.

## Features

- Add income and expenses with a category, date and note
- List all transactions, or only the ones for a given month
- Monthly report: totals, balance, and a breakdown by category with percentages
- Delete transactions by ID
- Import and export in CSV format
- Data is saved automatically to `data.csv`
- Input is validated: dates (including leap years), amounts, CSV lines

## Project structure

```
finance-manager/
├── CMakeLists.txt
├── include/
│   ├── date.h              # date struct, parsing, validation
│   ├── transaction.h       # transaction struct, money parsing
│   ├── finance_manager.h   # stores and edits transactions
│   ├── report.h            # monthly report and table output
│   └── csv_io.h            # CSV import/export
├── src/
│   ├── main.cpp            # menu and user input
│   ├── date.cpp
│   ├── transaction.cpp
│   ├── finance_manager.cpp
│   ├── report.cpp
│   └── csv_io.cpp
└── examples/
    └── sample.csv
```

## Build

You need CMake 3.16 or newer and a compiler with C++17 support (GCC, Clang, MSVC).

```bash
cmake -S . -B build
cmake --build build
```

## Run

Linux / macOS:
```bash
./build/finance
```

Windows:
```bash
build\Debug\finance.exe
```

## Example

```
===== Finance Manager =====
Balance: 65589.51
1. Add income
2. Add expense
...
> 5
Month (YYYY-MM, empty = current): 2026-09

===== Report for 2026-09 =====
Transactions: 8
Income:       50000.00
Expense:      17410.49
Balance:      32589.51

Income by category:
  salary              45000.00    90.0%
  freelance            5000.00    10.0%

Expense by category:
  rent                12000.00    68.9%
  food                 3190.50    18.3%
  education            1299.99     7.5%
  fun                   600.00     3.4%
  transport             320.00     1.8%
```

To try it with test data, choose `9. Import from CSV` and enter `examples/sample.csv`.

## CSV format

```
date;type;amount;category;note
2026-09-01;income;45000.00;salary;september
2026-09-03;expense;850.50;food;supermarket
```

- `date` - `YYYY-MM-DD`
- `type` - `income` or `expense`
- `amount` - positive number, up to 2 digits after the dot
- `note` - optional

Lines with errors are skipped, and the program reports which line failed and why.

## What I learned

- Splitting code into modules (`.h` / `.cpp`)
- `struct`, `enum class`, `class` with private data
- `std::vector`, `std::map`, `std::set`, and algorithms from `<algorithm>`
- Working with dates and validating them
- Reading and writing files with `fstream`
- Storing money as integers (kopecks) instead of `double`
- Building with CMake
