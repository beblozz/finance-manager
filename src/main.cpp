#include <iostream>
#include <string>

#include "csv_io.h"
#include "date.h"
#include "finance_manager.h"
#include "report.h"
#include "transaction.h"

const std::string DATA_FILE = "data.csv";

bool readLine(const std::string& prompt, std::string& line) {
    std::cout << prompt;
    if (!std::getline(std::cin, line)) {
        return false;
    }
    while (!line.empty() && (line.back() == ' ' || line.back() == '\r')) {
        line.pop_back();
    }
    while (!line.empty() && line.front() == ' ') {
        line.erase(0, 1);
    }
    return true;
}

std::string cleanText(std::string text) {
    for (char& c : text) {
        if (c == ';') {
            c = ',';
        }
    }
    return text;
}

bool readInt(const std::string& prompt, int& value) {
    std::string line;
    if (!readLine(prompt, line)) {
        return false;
    }
    try {
        size_t pos = 0;
        value = std::stoi(line, &pos);
        return pos == line.size();
    } catch (...) {
        return false;
    }
}

bool readMonth(int& year, int& month) {
    std::string line;
    if (!readLine("Month (YYYY-MM, empty = current): ", line)) {
        return false;
    }
    if (line.empty()) {
        Date now = today();
        year = now.year;
        month = now.month;
        return true;
    }
    Date d;
    if (!parseDate(line + "-01", d)) {
        std::cout << "Wrong month format.\n";
        return false;
    }
    year = d.year;
    month = d.month;
    return true;
}

void save(const FinanceManager& manager) {
    if (!saveToCsv(manager, DATA_FILE)) {
        std::cout << "Warning: could not save " << DATA_FILE << "\n";
    }
}

void addTransaction(FinanceManager& manager, TransactionType type) {
    Transaction t;
    t.type = type;

    std::string line;
    if (!readLine("Amount: ", line)) {
        return;
    }
    if (!parseMoney(line, t.amount) || t.amount == 0) {
        std::cout << "Wrong amount. Example: 1500 or 99.90\n";
        return;
    }

    if (!readLine("Category: ", line)) {
        return;
    }
    if (line.empty()) {
        std::cout << "Category can not be empty.\n";
        return;
    }
    t.category = cleanText(line);

    if (!readLine("Date (YYYY-MM-DD, empty = today): ", line)) {
        return;
    }
    if (line.empty()) {
        t.date = today();
    } else if (!parseDate(line, t.date)) {
        std::cout << "Wrong date.\n";
        return;
    }

    if (!readLine("Note (optional): ", line)) {
        return;
    }
    t.note = cleanText(line);

    int id = manager.add(t);
    save(manager);
    std::cout << "Added with id " << id << "\n";
}

void showMonth(const FinanceManager& manager) {
    int year = 0;
    int month = 0;
    if (!readMonth(year, month)) {
        return;
    }
    printTransactions(manager.byMonth(year, month));
}

void showReport(const FinanceManager& manager) {
    int year = 0;
    int month = 0;
    if (!readMonth(year, month)) {
        return;
    }
    printMonthReport(makeMonthReport(manager.all(), year, month));
}

void deleteTransaction(FinanceManager& manager) {
    int id = 0;
    if (!readInt("ID to delete: ", id)) {
        std::cout << "Wrong id.\n";
        return;
    }
    if (manager.remove(id)) {
        save(manager);
        std::cout << "Deleted.\n";
    } else {
        std::cout << "Transaction with id " << id << " not found.\n";
    }
}

void showCategories(const FinanceManager& manager) {
    std::vector<std::string> list = manager.categories();
    if (list.empty()) {
        std::cout << "No categories yet.\n";
        return;
    }
    for (const std::string& c : list) {
        std::cout << "  - " << c << "\n";
    }
}

void exportCsv(const FinanceManager& manager) {
    std::string path;
    if (!readLine("File name: ", path) || path.empty()) {
        return;
    }
    if (saveToCsv(manager, path)) {
        std::cout << "Exported " << manager.all().size() << " transactions to " << path << "\n";
    } else {
        std::cout << "Could not write file.\n";
    }
}

void importCsv(FinanceManager& manager) {
    std::string path;
    if (!readLine("File name: ", path) || path.empty()) {
        return;
    }
    ImportResult result;
    if (!loadFromCsv(manager, path, result)) {
        std::cout << "Could not open file.\n";
        return;
    }
    save(manager);
    std::cout << "Imported: " << result.loaded << "\n";
    if (!result.errors.empty()) {
        std::cout << "Skipped lines: " << result.errors.size() << "\n";
        for (const std::string& e : result.errors) {
            std::cout << "  " << e << "\n";
        }
    }
}

void printMenu(const FinanceManager& manager) {
    std::cout << "\n===== Finance Manager =====\n";
    std::cout << "Balance: " << formatMoney(manager.balance()) << "\n";
    std::cout << "1. Add income\n";
    std::cout << "2. Add expense\n";
    std::cout << "3. Show all transactions\n";
    std::cout << "4. Show transactions for month\n";
    std::cout << "5. Month report\n";
    std::cout << "6. Delete transaction\n";
    std::cout << "7. Show categories\n";
    std::cout << "8. Export to CSV\n";
    std::cout << "9. Import from CSV\n";
    std::cout << "0. Exit\n";
}

int main() {
    FinanceManager manager;

    ImportResult startup;
    if (loadFromCsv(manager, DATA_FILE, startup)) {
        std::cout << "Loaded " << startup.loaded << " transactions from " << DATA_FILE << "\n";
        if (!startup.errors.empty()) {
            std::cout << "Warning: " << startup.errors.size() << " broken lines were skipped\n";
        }
    }

    while (true) {
        printMenu(manager);
        int choice = -1;
        std::string line;
        if (!readLine("> ", line)) {
            break;
        }
        try {
            choice = std::stoi(line);
        } catch (...) {
            choice = -1;
        }

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            addTransaction(manager, TransactionType::Income);
        } else if (choice == 2) {
            addTransaction(manager, TransactionType::Expense);
        } else if (choice == 3) {
            printTransactions(manager.all());
        } else if (choice == 4) {
            showMonth(manager);
        } else if (choice == 5) {
            showReport(manager);
        } else if (choice == 6) {
            deleteTransaction(manager);
        } else if (choice == 7) {
            showCategories(manager);
        } else if (choice == 8) {
            exportCsv(manager);
        } else if (choice == 9) {
            importCsv(manager);
        } else {
            std::cout << "Unknown command.\n";
        }
    }

    save(manager);
    std::cout << "Bye!\n";
    return 0;
}
