#pragma once

#include <map>
#include <string>
#include <vector>

#include "transaction.h"

struct MonthReport {
    int year = 0;
    int month = 0;
    long long income = 0;
    long long expense = 0;
    int count = 0;
    std::map<std::string, long long> incomeByCategory;
    std::map<std::string, long long> expenseByCategory;
};

MonthReport makeMonthReport(const std::vector<Transaction>& transactions, int year, int month);
void printMonthReport(const MonthReport& report);
void printTransactions(const std::vector<Transaction>& transactions);
