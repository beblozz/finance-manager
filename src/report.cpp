#include "report.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>

MonthReport makeMonthReport(const std::vector<Transaction>& transactions, int year, int month) {
    MonthReport report;
    report.year = year;
    report.month = month;

    for (const Transaction& t : transactions) {
        if (t.date.year != year || t.date.month != month) {
            continue;
        }
        report.count++;
        if (t.type == TransactionType::Income) {
            report.income += t.amount;
            report.incomeByCategory[t.category] += t.amount;
        } else {
            report.expense += t.amount;
            report.expenseByCategory[t.category] += t.amount;
        }
    }
    return report;
}

static void printCategories(const std::map<std::string, long long>& data, long long total) {
    std::vector<std::pair<std::string, long long>> sorted(data.begin(), data.end());
    std::sort(sorted.begin(), sorted.end(),
        [](const std::pair<std::string, long long>& a, const std::pair<std::string, long long>& b) {
            return a.second > b.second;
        });

    for (const auto& item : sorted) {
        double percent = 0.0;
        if (total > 0) {
            percent = item.second * 100.0 / total;
        }
        std::cout << "  " << std::left << std::setw(16) << item.first
                  << std::right << std::setw(12) << formatMoney(item.second)
                  << std::setw(8) << std::fixed << std::setprecision(1) << percent << "%\n";
    }
}

void printMonthReport(const MonthReport& report) {
    std::cout << "\n===== Report for " << report.year << "-"
              << std::setw(2) << std::setfill('0') << report.month << std::setfill(' ')
              << " =====\n";

    if (report.count == 0) {
        std::cout << "No transactions in this month.\n";
        return;
    }

    std::cout << "Transactions: " << report.count << "\n";
    std::cout << "Income:       " << formatMoney(report.income) << "\n";
    std::cout << "Expense:      " << formatMoney(report.expense) << "\n";
    std::cout << "Balance:      " << formatMoney(report.income - report.expense) << "\n";

    if (!report.incomeByCategory.empty()) {
        std::cout << "\nIncome by category:\n";
        printCategories(report.incomeByCategory, report.income);
    }
    if (!report.expenseByCategory.empty()) {
        std::cout << "\nExpense by category:\n";
        printCategories(report.expenseByCategory, report.expense);
    }
}

void printTransactions(const std::vector<Transaction>& transactions) {
    if (transactions.empty()) {
        std::cout << "List is empty.\n";
        return;
    }

    std::cout << std::left
              << std::setw(5) << "ID"
              << std::setw(12) << "Date"
              << std::setw(9) << "Type"
              << std::setw(16) << "Category"
              << std::right << std::setw(12) << "Amount"
              << "  " << "Note" << "\n";
    std::cout << std::string(70, '-') << "\n";

    for (const Transaction& t : transactions) {
        std::cout << std::left
                  << std::setw(5) << t.id
                  << std::setw(12) << dateToString(t.date)
                  << std::setw(9) << typeToString(t.type)
                  << std::setw(16) << t.category
                  << std::right << std::setw(12) << formatMoney(t.amount)
                  << "  " << t.note << "\n";
    }
}
