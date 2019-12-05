#include "util.h"
#include <cassert>
#include <regex>

using point = std::tuple<int, int>;

int main() {
    std::regex r("#\\d+ @ (\\d+),(\\d+): (\\d+)x(\\d+)");
    map<point, unsigned int> grid;
    map<point, unsigned int> grid_ownership;
    set<unsigned int> dirty_claims;

    auto lines = util::read_lines("d3_input.txt");
    for (unsigned int idx = 0; idx < lines.size(); idx++) {
        std::smatch match;

        assert(std::regex_match(lines[idx], match, r));
        int x = std::stoi(match[1].str());
        int y = std::stoi(match[2].str());
        int width = std::stoi(match[3].str());
        int height = std::stoi(match[4].str());
        for (int i = x; i < x + width; i++) {
            for (int j = y; j < y + height; j++) {
                auto p = std::make_tuple(i, j);
                grid[p]++;

                auto entry = grid_ownership.find(p);

                if (entry != grid_ownership.end()) {
                    dirty_claims.insert(entry->second);
                    dirty_claims.insert(idx + 1);
                }
                grid_ownership[p] = idx + 1;
            }
        }
    }

    int count_overlaps = 0;
    for (const auto& e : grid) {
        if (e.second >= 2) {
            count_overlaps++;
        }
    }
    std::cout << "p1: " << count_overlaps << std::endl;

    for (unsigned int i = 1; i <= lines.size(); i++) {
        if (dirty_claims.count(i) == 0) {
            std::cout << "p2: " << i << std::endl;
        }
    }
	return 0;
}
