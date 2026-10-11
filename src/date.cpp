#include "date.h"

#include <cstdio>
#include <ctime>

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int daysInMonth(int year, int month) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return days[month - 1];
}

bool isValidDate(const Date& d) {
    if (d.year < 1900 || d.year > 2100) {
        return false;
    }
    if (d.month < 1 || d.month > 12) {
        return false;
    }
    return d.day >= 1 && d.day <= daysInMonth(d.year, d.month);
}

bool parseDate(const std::string& text, Date& result) {
    if (text.size() != 10 || text[4] != '-' || text[7] != '-') {
        return false;
    }
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            continue;
        }
        if (text[i] < '0' || text[i] > '9') {
            return false;
        }
    }

    Date d;
    d.year = std::stoi(text.substr(0, 4));
    d.month = std::stoi(text.substr(5, 2));
    d.day = std::stoi(text.substr(8, 2));

    if (!isValidDate(d)) {
        return false;
    }
    result = d;
    return true;
}

std::string dateToString(const Date& d) {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d", d.year, d.month, d.day);
    return buf;
}

Date today() {
    std::time_t now = std::time(nullptr);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif
    Date d;
    d.year = local.tm_year + 1900;
    d.month = local.tm_mon + 1;
    d.day = local.tm_mday;
    return d;
}

bool operator<(const Date& a, const Date& b) {
    if (a.year != b.year) {
        return a.year < b.year;
    }
    if (a.month != b.month) {
        return a.month < b.month;
    }
    return a.day < b.day;
}

bool operator==(const Date& a, const Date& b) {
    return a.year == b.year && a.month == b.month && a.day == b.day;
}
