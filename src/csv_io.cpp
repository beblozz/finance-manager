#include "csv_io.h"

#include <fstream>
#include <sstream>

static std::vector<std::string> split(const std::string& line, char sep) {
    std::vector<std::string> parts;
    std::string current;
    std::stringstream ss(line);
    while (std::getline(ss, current, sep)) {
        parts.push_back(current);
    }
    if (!line.empty() && line.back() == sep) {
        parts.push_back("");
    }
    return parts;
}

bool saveToCsv(const FinanceManager& manager, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }

    file << "date;type;amount;category;note\n";
    for (const Transaction& t : manager.all()) {
        file << dateToString(t.date) << ";"
             << typeToString(t.type) << ";"
             << formatMoney(t.amount) << ";"
             << t.category << ";"
             << t.note << "\n";
    }
    return true;
}

bool loadFromCsv(FinanceManager& manager, const std::string& path, ImportResult& result) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }
        if (lineNumber == 1 && line.rfind("date;", 0) == 0) {
            continue;
        }

        std::vector<std::string> parts = split(line, ';');
        if (parts.size() < 4 || parts.size() > 5) {
            result.errors.push_back("line " + std::to_string(lineNumber) + ": wrong number of fields");
            continue;
        }

        Transaction t;
        if (!parseDate(parts[0], t.date)) {
            result.errors.push_back("line " + std::to_string(lineNumber) + ": bad date");
            continue;
        }
        if (!parseType(parts[1], t.type)) {
            result.errors.push_back("line " + std::to_string(lineNumber) + ": bad type");
            continue;
        }
        if (!parseMoney(parts[2], t.amount) || t.amount == 0) {
            result.errors.push_back("line " + std::to_string(lineNumber) + ": bad amount");
            continue;
        }
        if (parts[3].empty()) {
            result.errors.push_back("line " + std::to_string(lineNumber) + ": empty category");
            continue;
        }
        t.category = parts[3];
        if (parts.size() == 5) {
            t.note = parts[4];
        }

        manager.add(t);
        result.loaded++;
    }
    return true;
}
