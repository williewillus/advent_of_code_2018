#include "util.h"
#include <cassert>
#include <regex>

struct Star {
	std::tuple<int, int> pos;
	const std::tuple<int, int> vel;

	void tick() {
		std::get<0>(pos) += std::get<0>(vel);
		std::get<1>(pos) += std::get<1>(vel);
	}
};

int main() {
	std::regex re("position=<((?: |-)\\d+), ((?: |-)\\d+)> velocity=<((?: |-)\\d+), ((?: |-)\\d+)>");
	vec<Star> stars;
	for (const auto& str : util::read_lines("d10_input.txt")) {
		std::smatch match;
		assert(std::regex_match(str, match, re));
		int px = std::stoi(match[1].str());
		int py = std::stoi(match[2].str());
		int vx = std::stoi(match[3].str());
		int vy = std::stoi(match[4].str());
		stars.push_back({{px, py}, {vx, vy}});
	}
	return 0;
}
