#pragma once

#include <string>

struct Date {
    int year = 0;
    int month = 0;
    int day = 0;
};

bool isLeapYear(int year);
int daysInMonth(int year, int month);
bool isValidDate(const Date& d);
bool parseDate(const std::string& text, Date& result);
std::string dateToString(const Date& d);
Date today();
bool operator<(const Date& a, const Date& b);
bool operator==(const Date& a, const Date& b);
