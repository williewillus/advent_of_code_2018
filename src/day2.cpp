#include "util.h"
#include <regex>

namespace day2 {

static std::tuple<bool, bool> count_double_triple(const string& s) {
    map<char, long> m;
    for (char c : s) {
        m[c]++;
    }
    bool has_double = false;
    bool has_triple = false;
    for (const auto& pair : m) {
        if (pair.second == 2) {
            has_double = true;
        } else if (pair.second == 3) {
            has_triple = true;
        }
    }
    return std::make_tuple(has_double, has_triple);
}

static bool has_one_diff(const string& s1, const string& s2) {
    long diffs = 0;
    for (unsigned int i = 0; i < s1.length(); i++) {
        if (s1[i] != s2[i])
            diffs++;
    }
    return diffs == 1;
}

void run() {
    vec<string> lines = util::read_input<string>("d2_input.txt");
    long count_double = 0, count_triple = 0;
    for (const auto& s : lines) {
        auto [has_double, has_triple] = count_double_triple(s);
        if (has_double)
                count_double++;
        if (has_triple)
                count_triple++;
    }
    std::cout << "p1: " << count_double * count_triple << std::endl;

    for (unsigned int i = 0; i < lines.size(); i++) {
        for (unsigned int j = i + 1; j < lines.size(); j++) {
            if (has_one_diff(lines[i], lines[j])) {
                std::cout << "p2:" << std::endl;
                std::cout << lines[i] << std::endl;
                std::cout << lines[j] << std::endl;
                return;
            }
        }
    }
}
}
