#include "transaction.h"

std::string typeToString(TransactionType type) {
    if (type == TransactionType::Income) {
        return "income";
    }
    return "expense";
}

bool parseType(const std::string& text, TransactionType& result) {
    if (text == "income") {
        result = TransactionType::Income;
        return true;
    }
    if (text == "expense") {
        result = TransactionType::Expense;
        return true;
    }
    return false;
}

bool parseMoney(const std::string& text, long long& result) {
    if (text.empty()) {
        return false;
    }

    long long whole = 0;
    long long cents = 0;
    int centDigits = 0;
    bool afterDot = false;
    bool hasDigits = false;

    for (char c : text) {
        if (c == '.' || c == ',') {
            if (afterDot) {
                return false;
            }
            afterDot = true;
        } else if (c >= '0' && c <= '9') {
            hasDigits = true;
            if (afterDot) {
                if (centDigits == 2) {
                    return false;
                }
                cents = cents * 10 + (c - '0');
                centDigits++;
            } else {
                if (whole > 100000000000LL) {
                    return false;
                }
                whole = whole * 10 + (c - '0');
            }
        } else {
            return false;
        }
    }

    if (!hasDigits) {
        return false;
    }
    if (centDigits == 1) {
        cents *= 10;
    }

    result = whole * 100 + cents;
    return true;
}

std::string formatMoney(long long amount) {
    std::string sign;
    if (amount < 0) {
        sign = "-";
        amount = -amount;
    }
    long long whole = amount / 100;
    long long cents = amount % 100;
    std::string centsText = std::to_string(cents);
    if (cents < 10) {
        centsText = "0" + centsText;
    }
    return sign + std::to_string(whole) + "." + centsText;
}
