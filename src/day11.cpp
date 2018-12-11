#include "util.h"
#include <array>
#include <limits>

namespace day11 {
static constexpr int64_t INPUT = 5093;

static int64_t power(int64_t x, int64_t y) {
	auto rack = x + 10;
	auto pow = rack * y;
	pow += INPUT;
	pow *= rack;
	auto hundreds = (pow/100) % 10;
	return hundreds - 5;
}

static void run_p2(const std::array<std::array<int64_t, 300>, 300>& grid) {
	int64_t max_score = std::numeric_limits<int64_t>::min();
	uint32_t max_row, max_col, size;

	for (uint32_t row = 1; row <= 300; row++) {
		for (uint32_t col = 1; col <= 300; col++) {
			int64_t local_score = 0;
			// try expanding rightward and downward
			for (uint32_t expansion = 0; row+expansion <= 300 && col+expansion <= 300; expansion++) {
				// right edge excluding the corner
				for (uint32_t new_row = row; new_row <= row + expansion - 1; new_row++) {
					uint32_t new_col = col + expansion;
					local_score += grid.at(new_col-1).at(new_row-1);
				}

				// bottom edge including the corner
				for (uint32_t new_col = col; new_col <= col + expansion; new_col++) {
					uint32_t new_row = row + expansion;
					local_score += grid.at(new_col-1).at(new_row-1);
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

void run() {
	// ~720K, should be fine on linux...
	std::array<std::array<int64_t, 300>, 300> grid;
	for (uint32_t x = 1; x <= 300; x++) {
		for (uint32_t y = 1; y <= 300; y++) {
			grid.at(x-1).at(y-1) = power(x, y);
		}
	}

	int64_t max_pow = std::numeric_limits<int64_t>::min();
	uint32_t max_row, max_col;

	// Only gonna slide window in one dimension bc lazy
	for (uint32_t row = 1; row <= 298; row++) {
		int64_t sliding_score;

		// sliding into `col` each iteration
		for (uint32_t col = 1; col <= 298; col++) {
			// note: need to account for 1-indexing when accessing grid
			if (col == 1) {
				// first column, just set up
				sliding_score = grid.at(0).at(row -1) + grid.at(1).at(row -1) + grid.at(2).at(row -1)
						+ grid.at(0).at(row + 1 -1) + grid.at(1).at(row + 1 -1) + grid.at(2).at(row + 1 -1)
						+ grid.at(0).at(row + 2 -1) + grid.at(1).at(row + 2 -1) + grid.at(2).at(row + 2 -1);
			} else {

				// drop everything in col-1
				sliding_score -= grid.at(col-1 -1).at(row -1);
				sliding_score -= grid.at(col-1 -1).at(row+1 -1);
				sliding_score -= grid.at(col-1 -1).at(row+2 -1);

				// add everything in col+2
				sliding_score += grid.at(col+2 -1).at(row -1);
				sliding_score += grid.at(col+2 -1).at(row+1 -1);
				sliding_score += grid.at(col+2 -1).at(row+2 -1);
			}

			if (sliding_score > max_pow) {
				max_pow = sliding_score;
				max_row = row;
				max_col = col;
			}
		}
	}
	std::cout << "p1: " << max_col << ", " << max_row << std::endl;
	run_p2(grid);
}
}
