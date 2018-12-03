#include "util.h"

namespace util {
vec<string> read_lines(const string& filename) {
    vec<string> ret;
    std::ifstream f(filename);
    std::string line;
    while (std::getline(f, line)) {
        ret.push_back(std::move(line));
    }
    return ret;
}
}
