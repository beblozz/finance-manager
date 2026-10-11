#include "finance_manager.h"

#include <algorithm>
#include <set>

int FinanceManager::add(Transaction t) {
    t.id = nextId;
    nextId++;

    auto pos = std::upper_bound(transactions.begin(), transactions.end(), t,
        [](const Transaction& a, const Transaction& b) {
            return a.date < b.date;
        });
    transactions.insert(pos, t);
    return t.id;
}

bool FinanceManager::remove(int id) {
    auto it = std::find_if(transactions.begin(), transactions.end(),
        [id](const Transaction& t) {
            return t.id == id;
        });
    if (it == transactions.end()) {
        return false;
    }
    transactions.erase(it);
    return true;
}

void FinanceManager::clear() {
    transactions.clear();
    nextId = 1;
}

const std::vector<Transaction>& FinanceManager::all() const {
    return transactions;
}

std::vector<Transaction> FinanceManager::byMonth(int year, int month) const {
    std::vector<Transaction> result;
    for (const Transaction& t : transactions) {
        if (t.date.year == year && t.date.month == month) {
            result.push_back(t);
        }
    }
    return result;
}

std::vector<std::string> FinanceManager::categories() const {
    std::set<std::string> unique;
    for (const Transaction& t : transactions) {
        unique.insert(t.category);
    }
    return std::vector<std::string>(unique.begin(), unique.end());
}

long long FinanceManager::balance() const {
    long long total = 0;
    for (const Transaction& t : transactions) {
        if (t.type == TransactionType::Income) {
            total += t.amount;
        } else {
            total -= t.amount;
        }
    }
    return total;
}
