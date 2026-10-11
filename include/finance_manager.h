#pragma once

#include <string>
#include <vector>

#include "transaction.h"

class FinanceManager {
public:
    int add(Transaction t);
    bool remove(int id);
    void clear();

    const std::vector<Transaction>& all() const;
    std::vector<Transaction> byMonth(int year, int month) const;
    std::vector<std::string> categories() const;
    long long balance() const;

private:
    std::vector<Transaction> transactions;
    int nextId = 1;
};
