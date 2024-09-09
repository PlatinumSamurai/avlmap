// Copyright (c) 2024 PlatinumSamurai. All rights reserved.

#ifndef FORMAT_FORMAT_FORMAT_HPP_
#define FORMAT_FORMAT_FORMAT_HPP_

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <set>
#include <algorithm>
#include <sstream>
#include <numeric>
#include "error.hpp"


using PositionsList = std::vector<std::pair<int, int>>;

static PositionsList findNextInPattern(const std::string &str,
                                                        unsigned int &order) {
    size_t i = 0;
    size_t j = 0;
    unsigned int temp;
    PositionsList positions;

    while (i < str.size() && j < str.size()) {
        i = str.find('{', i);
        j = str.find('}', j);
        if (i < j && j != std::string::npos) {
            std::string substr = str.substr(i + 1, j - i - 1);
            if (!std::all_of(substr.begin(), substr.end(),
                        [](char ch) {return std::isdigit(ch);})) {
                throw Error("invalid value in {" + substr +
                                "}. Only positive integersare allowed");
            }
            temp = std::stoi(str.substr(i + 1, j));
            if (temp == order) {
                positions.emplace_back(std::make_pair(i, j));
            }
        } else {
            break;
        }
        i++;
        j++;
    }
    order++;

    return positions;
}

static int CountUniqueParameters(const std::string &str) {
    size_t i = 0;
    size_t j = 0;
    unsigned int sum = 0;
    std::set<unsigned int> uniqueNumbers;

    while (i < str.size() && j < str.size()) {
        i = str.find('{', i);
        j = str.find('}', j);

        if (i < j && j != std::string::npos) {
            std::string substr = str.substr(i + 1, j - i - 1);
            if (!std::all_of(substr.begin(), substr.end(),
                        [](char ch) {return std::isdigit(ch);})) {
                throw Error("invalid value in {" + substr +
                            "}. Only positive intergers are allowed");
            }
            uniqueNumbers.insert(std::stoi(substr));
        } else {
            break;
        }
        i++;
        j++;
    }
    sum = std::accumulate(uniqueNumbers.begin(), uniqueNumbers.end(), 0);

    if ((sum != uniqueNumbers.size() * (uniqueNumbers.size() - 1) / 2) ||
                                              *uniqueNumbers.begin() != 0) {
        throw Error("Invalid numeration of template parameters");
    }

    return uniqueNumbers.size();
}

template <typename T>
concept HasOutputOperator = requires(std::ostream &out, T t) {
        out << t;
};

template <typename T>
std::string format(const std::string&, unsigned&, T&&);

template <typename T, typename... Args>
std::string format(const std::string &, unsigned&, T &&, Args&&...);


template <HasOutputOperator T, HasOutputOperator... Args>
std::string format(const std::string &pattern, T &&val, Args&&... args) {
    unsigned int order = 0;
    int count = CountUniqueParameters(pattern);

    if (count != sizeof...(args) + 1) {
        throw Error("Invalid number of parameters");
    }

    return format(pattern, order, std::forward<T>(val),
                                                std::forward<Args>(args)...);
}


template <typename T, typename... Args>
std::string format(const std::string &pattern, unsigned &order,
                                                    T &&val, Args&&... args) {
    std::ostringstream ostr;

    ostr << format(pattern, order, std::forward<T>(val));

    return format(ostr.str(), order, std::forward<Args>(args)...);
}


template <typename T>
std::string format(const std::string &pattern, unsigned &order, T &&val) {
    std::ostringstream ostr;
    PositionsList positions = findNextInPattern(pattern, order);
    int j = 0;

    for (size_t i = 0; i < positions.size(); ++i) {
        ostr << pattern.substr(j, positions.at(i).first - j);
        ostr << val;
        j = positions.at(i).second + 1;
    }
    ostr << pattern.substr(j);

    return ostr.str();
}

#endif  // FORMAT_FORMAT_FORMAT_HPP_

