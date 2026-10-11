#pragma once

#include <string>

#include "date.h"

enum class TransactionType {
    Income,
    Expense
};

struct Transaction {
    int id = 0;
    Date date;
    TransactionType type = TransactionType::Expense;
    long long amount = 0;
    std::string category;
    std::string note;
};

std::string typeToString(TransactionType type);
bool parseType(const std::string& text, TransactionType& result);
bool parseMoney(const std::string& text, long long& result);
std::string formatMoney(long long amount);
