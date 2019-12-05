#include "util.h"
#include <cmath>
#include <deque>
#include <limits>
#include <map>

// idea: bfs from each anchor point, stopping at edge and replacing owners with min as necessary
	using point = std::tuple<int, int>;

	static int manhattan(const point& p1, const point& p2) {
		return std::abs(std::get<0>(p1) - std::get<0>(p2)) + std::abs(std::get<1>(p1) - std::get<1>(p2));
	}

	int main() {
		vec<point> anchors;
		int min_x = std::numeric_limits<int>::max();
		int min_y = std::numeric_limits<int>::max();
		int max_x = std::numeric_limits<int>::min();
		int max_y = std::numeric_limits<int>::min();

		for (const auto& str : util::read_lines("d6_input.txt")) {
			auto comma = str.find(",");
			auto x = std::stoi(str.substr(0, comma));
			auto y = std::stoi(str.substr(comma+1));
			if (x < min_x)
				min_x  = x;
			if (y < min_y)
				min_y = y;
			if (x > max_x)
				max_x = x;
			if (y > max_y)
				max_y = y;
			anchors.emplace_back(x, y);
		}
		std::cout << min_x << ", " << max_x << std::endl;
		std::cout << min_y << ", " << max_y << std::endl;
		
		static constexpr unsigned int TIE = 0xDEADBEEF;
		map<point, unsigned int> owners;

		// for each point of interest, calculate owner
		for (int i = min_x; i <= max_x; i++) {
			for (int j = min_y; j <= max_y; j++) {
				auto point = std::make_tuple(i, j);

				// gather distances of all anchors
				std::map<int, vec<unsigned int>> dist_to_anchors;
				for (unsigned int idx = 0; idx < anchors.size(); idx++) {
					const auto& anch = anchors.at(idx);
					auto dist = manhattan(anch, point);
					dist_to_anchors[dist].push_back(idx);
				}

				// if lowest distance has more than one anchor, it's a tie, otherwise set owner
				auto closest = dist_to_anchors.begin();
				if (closest->second.size() > 1) {
					owners[point] = TIE;
				} else {
					owners[point] = closest->second.at(0);
				}
			}
		}

		// remove all infinite anchors (anchors with members present on the outer ring of the area of interest
		set<unsigned int> finite_anchors;
		for (unsigned int i = 0; i < anchors.size(); i++) { finite_anchors.insert(i); }
		for (int i = min_x; i <= max_x; i++) {
			auto top = point(i, min_y);
			auto bottom = point(i, max_y);
			auto top_owner = owners.at(top);
			auto bottom_owner = owners.at(bottom);
			if (top_owner != TIE)
				finite_anchors.erase(top_owner);
			if (bottom_owner != TIE)
				finite_anchors.erase(bottom_owner);
		}
		for (int j = min_y; j <= max_y; j++) {
			auto left = point(min_x, j);
			auto right = point(max_x, j);
			auto left_owner = owners.at(left);
			auto right_owner = owners.at(right);
			if (left_owner != TIE)
				finite_anchors.erase(left_owner);
			if (right_owner != TIE)
				finite_anchors.erase(right_owner);
		}

		unsigned int max_area = 0;
		for (unsigned int anch : finite_anchors) {
			unsigned int area = 0;
			for (const auto& e : owners) {
				if (e.second == anch)
					area++;
			}
			if (area > max_area)
				max_area = area;
		}
		std::cout << "p1: " << max_area << std::endl;

		unsigned int valid_points = 0;
		for (int i = min_x; i <= max_x; i++) {
			for (int j = min_y; j <= max_y; j++) {
				auto p = point(i, j);
				int dist_sum = 0;
				for (const auto& anch : anchors) {
					dist_sum += manhattan(anch, p);
				}
				if (dist_sum < 10000)
					valid_points++;
			}
		}
		std::cout << "p2: " << valid_points << std::endl;
		return 0;
	}
