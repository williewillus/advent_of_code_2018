#include "util.h"
#include <array>
#include <limits>

static constexpr int INPUT = 5093;

static int power(int x, int y) {
	auto rack = x + 10;
	auto pow = rack * y;
	pow += INPUT;
	pow *= rack;
	auto hundreds = (pow/100) % 10;
	return hundreds - 5;
}

static void run_p1(const std::array<std::array<int, 300>, 300>& grid) {
	int max_pow = std::numeric_limits<int>::min();
	uint32_t max_row, max_col;

	for (uint32_t row = 1; row <= 298; row++) {
		for (uint32_t col = 1; col <= 298; col++) {
			int score = grid[col-1][row-1] + grid[col][row-1] + grid[col+1][row-1]
						+ grid[col-1][row] + grid[col][row] + grid[col+1][row]
						+ grid[col-1][row+1] + grid[col][row+1] + grid[col+1][row+1];

			if (score > max_pow) {
				max_pow = score;
				max_row = row;
				max_col = col;
			}
		}
	}
	std::cout << "p1: " << max_col << "," << max_row << std::endl;
}

static void run_p2(const std::array<std::array<int, 300>, 300>& grid) {
	int max_score = std::numeric_limits<int>::min();
	uint32_t max_row, max_col, size;

	for (uint32_t row = 1; row <= 300; row++) {
		for (uint32_t col = 1; col <= 300; col++) {
			int local_score = 0;
			// try expanding rightward and downward
			for (uint32_t expansion = 0; row+expansion <= 300 && col+expansion <= 300; expansion++) {
				// right edge excluding the corner
				for (uint32_t new_row = row; new_row <= row + expansion - 1; new_row++) {
					uint32_t new_col = col + expansion;
					local_score += grid[new_col-1][new_row-1];
				}

				// bottom edge including the corner
				for (uint32_t new_col = col; new_col <= col + expansion; new_col++) {
					uint32_t new_row = row + expansion;
					local_score += grid[new_col-1][new_row-1];
				}

				if (local_score > max_score) {
					max_score = local_score;
					max_row = row;
					max_col = col;
					size = expansion + 1;
				}
			}
		}
	}

	std::cout << "p2: " << max_col << "," << max_row << "," << size << std::endl;
}

int main() {
	// ~351K, should be fine...
	std::array<std::array<int, 300>, 300> grid;
	for (uint32_t x = 1; x <= 300; x++) {
		for (uint32_t y = 1; y <= 300; y++) {
			grid[x-1][y-1] = power(x, y);
		}
	}

	run_p1(grid);
	run_p2(grid);
	return 0;
}
