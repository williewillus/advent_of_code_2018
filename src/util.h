#ifndef UTIL_H
#define UTIL_H

// a bunch of prelude stuff
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <set>
#include <map>
#include <vector>

template<typename K, typename V>
using map = std::map<K, V>;

template<typename T>
using set = std::set<T>;

using string = std::string;

template<typename T>
using vec = std::vector<T>;

namespace util {
template<typename T>
vec<T> read_input(const string& filename) {
    std::ifstream f(filename);
    vec<T> ret;
    std::copy(std::istream_iterator<T>(f), {}, std::back_inserter(ret));
    return ret;
}

vec<string> read_lines(const string& filename);
}

#endif // UTIL_H
