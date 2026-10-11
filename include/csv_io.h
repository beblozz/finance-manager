#pragma once

#include <string>
#include <vector>

#include "finance_manager.h"

struct ImportResult {
    int loaded = 0;
    std::vector<std::string> errors;
};

bool saveToCsv(const FinanceManager& manager, const std::string& path);
bool loadFromCsv(FinanceManager& manager, const std::string& path, ImportResult& result);
