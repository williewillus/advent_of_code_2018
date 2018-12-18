#include "util.h"
#include <cassert>

namespace day18 {
enum tile : uint8_t {
	EMPTY,
	TREE,
	LUMBERYARD,
};

static void show(const vec<vec<tile>>& grid) {
	for (uint32_t y = 0; y < grid.size(); y++) {
		for (uint32_t x = 0; x < grid[y].size(); x++) {
			switch (grid[y][x]) {
			case TREE: std::cout << '|'; break;
			case LUMBERYARD: std::cout << '#'; break;
			case EMPTY: std::cout << '.'; break;
			default: break;
			}
		}
		std::cout << std::endl;
	}
}

static uint32_t resource_value(const vec<vec<tile>>& grid) {
	uint32_t count_tree = 0;
	uint32_t count_lumberyard = 0;
	for (uint32_t y = 0; y < grid.size(); y++) {
		for (uint32_t x = 0; x < grid[y].size(); x++) {
			switch (grid[y][x]) {
			case TREE: count_tree++; break;
			case LUMBERYARD: count_lumberyard++; break;
			default: break;
			}
		}
	}
	return count_tree * count_lumberyard;
}

static constexpr std::tuple<uint32_t, uint32_t> DELTAS[8] = {
	{-1, 0}, {1, 0}, {0, -1}, {0, 1},
	{-1, -1}, {-1, 1}, {1, -1}, {1, 1}
};

static std::tuple<int, int, int> count_adjacent(const vec<vec<tile>>& grid, uint32_t y, uint32_t x) {
	int count_empty = 0;
	int count_tree = 0;
	int count_lumberyard = 0;
	for (auto [dx, dy] : DELTAS) {
		auto nby = y + dy;
		auto nbx = x + dx;
		if (nby < grid.size() && nbx < grid[nby].size()) {
			switch (grid[nby][nbx]) {
			case EMPTY: count_empty++; break;
			case TREE: count_tree++; break;
			case LUMBERYARD: count_lumberyard++; break;
			}
		}
	}
	return {count_empty, count_tree, count_lumberyard};
}

static void tick(const vec<vec<tile>>& src, vec<vec<tile>>& dest) {
	for (uint32_t y = 0; y < src.size(); y++) {
		for (uint32_t x = 0; x < src[y].size(); x++) {
			const auto [count_empty, count_tree, count_lumberyard] = count_adjacent(src, y, x);

			switch (src[y][x]) {
			case EMPTY: {
				dest[y][x] = count_tree >= 3 ? TREE : EMPTY;
				break;
			}
			case TREE: {
				dest[y][x] = count_lumberyard >= 3 ? LUMBERYARD : TREE;
				break;
			}
			case LUMBERYARD: {
				dest[y][x] = (count_tree >= 1 && count_lumberyard >= 1) ? LUMBERYARD : EMPTY;
				break;
			}
			}
		}
	}
}

void run() {
	vec<vec<tile>> grid;
	for (const auto& line : util::read_lines("d18_input.txt")) {
		vec<tile> row;
		for (char c : line) {
			switch (c) {
			case '.': row.push_back(EMPTY); break;
			case '|': row.push_back(TREE); break;
			case '#': row.push_back(LUMBERYARD); break;
			}
		}
		grid.push_back(std::move(row));
	}

	auto grid2 = grid;

	for (int i = 0; i < 10; i++) {
		tick(grid, grid2);
		std::swap(grid, grid2);
	}
	std::cout << "p1: " << resource_value(grid) << std::endl;
	
	// Assume 200k is enough to stabilize
	for (uint32_t i = 10; i < 200000; i++) {
		tick(grid, grid2);
		std::swap(grid, grid2);
		if (i > 199900) {
			std::cout << (i+1) << ": " << resource_value(grid) << std::endl;
		}
	}
	std::cout << "p2: " << "eyeball the period from the sequence above, arbitrarily choosing a time and value to start the cycle with" << std::endl;
	std::cout << "Then, do (1000000000 - <start time>) % <period>. Take the result as a 0-indexed value into the repeating sequence as the answer." << std::endl;
	std::cout << "On the committed input in this repo, should yield answer 202806" << std::endl;
}
}
