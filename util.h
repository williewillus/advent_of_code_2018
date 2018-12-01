#ifndef UTIL_H
#define UTIL_H

// a bunch of prelude stuff
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace util {
std::vector<long> read_as_ints(std::ifstream& file);
}

#endif // UTIL_H
