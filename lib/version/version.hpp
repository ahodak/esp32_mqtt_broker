/*
 *  Библиотека для работы с версиями
 *  -----------------------------------------------------------------------------------------------------------------------
 *  (c) 2025 Ходак Андрей | Andrey Khodak
 *  andrey@khodak.ru | https://khodak.ru | tg: @akhodak
*/
#pragma once

#include <iostream>
#include <sstream>
#include <vector>

namespace Version {
    inline std::vector<int> parseVersion(const std::string& version) {
        std::vector<int> components;
        std::stringstream ss(version);
        std::string token;

        while (std::getline(ss, token, '.')) {
            components.push_back(std::stoi(token));
        }

        return components;
    }

    inline int compareVersions(const std::string& version1, const std::string& version2) {
        std::vector<int> v1 = parseVersion(version1);
        std::vector<int> v2 = parseVersion(version2);

        size_t maxSize = std::max(v1.size(), v2.size());

        v1.resize(maxSize, 0);
        v2.resize(maxSize, 0);

        for (size_t i = 0; i < maxSize; ++i) {
            if (v1[i] < v2[i]) return -1;
            if (v1[i] > v2[i]) return 1;
        }

        return 0;
    }
}