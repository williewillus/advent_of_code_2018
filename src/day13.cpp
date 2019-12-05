#include "util.h"
#include <cassert>
#include <optional>

enum Track : uint8_t {
	NONE,
	HORIZ,
	VERT,
	LSLASH,
	RSLASH,
	JUNCTION,
};

enum Direction : uint8_t {
	NORTH,
	SOUTH,
	WEST,
	EAST,
};

static Direction cw(Direction d) {
	switch (d) {
		case NORTH: return EAST;
		case SOUTH: return WEST;
		case EAST: return SOUTH;
		case WEST: return NORTH;
		default: throw "impossible";
	}
}

static Direction ccw(Direction d) {
	switch (d) {
		case NORTH: return WEST;
		case SOUTH: return EAST;
		case EAST: return NORTH;
		case WEST: return SOUTH;
		default: throw "impossible";
	}
}

struct Cart {
	Direction dir = NORTH;
	int turn_counter = 0;
	bool updated = false;
};

using ocart = std::optional<Cart>;
using point = std::tuple<uint32_t, uint32_t>;

static std::optional<point> tick(const vec<vec<Track>>& map, vec<vec<ocart>>& carts, bool p2) {
	for (uint32_t y = 0; y < carts.size(); y++) {
		for (uint32_t x = 0; x < carts[y].size(); x++) {
			if (!carts[y][x]) continue;

			// skip if we've already updated this cart this tick
			if (carts[y][x]->updated) continue;
			carts[y][x]->updated = true;

			// where are we trying to go?
			uint32_t facing_y = y, facing_x = x;
			auto old_dir = carts[y][x]->dir;
			switch(old_dir) {
				case NORTH: { facing_y = y - 1; break; }
				case SOUTH: { facing_y = y + 1; break; }
				case EAST: { facing_x = x + 1; break; }
				case WEST: { facing_x = x - 1; break; }
			}

			// check for collisions
			if (carts.at(facing_y).at(facing_x)) {
				if (p2) {
					carts[facing_y][facing_x].reset();
					carts[y][x].reset();
					continue;
				} else return {{facing_x, facing_y}};
			}

			// move
			carts[facing_y][facing_x] = std::move(carts[y][x]);
			carts[y][x].reset();

			// update facing
			auto& cart = *carts[facing_y][facing_x];
			switch (map.at(facing_y).at(facing_x)) {
				case LSLASH: {
					if (old_dir == NORTH || old_dir == SOUTH)
						cart.dir = cw(old_dir);
					else cart.dir = ccw(old_dir);
					break;
				}
				case RSLASH: {
					if (old_dir == NORTH || old_dir == SOUTH)
						cart.dir = ccw(old_dir);
					else cart.dir = cw(old_dir);
					break;
				}
				case JUNCTION: {
					switch(cart.turn_counter) {
						case 0: cart.dir = ccw(old_dir); break;
						case 1: break;
						case 2: cart.dir = cw(old_dir); break;
					}
					cart.turn_counter = (cart.turn_counter + 1) % 3;
					break;
				}
				case NONE: throw "RAN OFF TRACKS, IMPOSSIBLE";
				default: break;
			}
		}
	}
	return {};
}

static void reset_updated(vec<vec<ocart>>& carts) {
	for (uint32_t y = 0; y < carts.size(); y++) {
		for (uint32_t x = 0; x < carts[y].size(); x++) {
			if (carts[y][x])
				carts[y][x]->updated = false;
		}
	}
}

int main() {
	auto lines = util::read_lines("d13_input.txt");

	vec<vec<Track>> map;
	vec<vec<ocart>> carts;
	for (uint32_t y = 0; y < lines.size(); y++) {
		vec<Track> row_tracks;
		vec<ocart> row_carts;

		const auto& line = lines[y];
		for (uint32_t x = 0; x < lines[y].size(); x++) {
			Track t = NONE;
			ocart c = {};
			switch (line[x]) {
				case '/': { t = LSLASH;	break; }
				case '\\': { t = RSLASH; break; }
				case '-': { t = HORIZ; break; }
				case '|': { t = VERT; break; }
				case '+': { t = JUNCTION; break; }
				case '^': { t = VERT; c = {Cart()}; c->dir = NORTH; break; }
				case 'v': { t = VERT; c = {Cart()}; c->dir = SOUTH; break; }
				case '>': { t = HORIZ; c = {Cart()}; c->dir = EAST; break; }
				case '<': { t = HORIZ; c = {Cart()}; c->dir = WEST; break; }
			}
			row_tracks.push_back(t);
			row_carts.push_back(c);
		}

		map.push_back(std::move(row_tracks));
		carts.push_back(std::move(row_carts));
	}

	auto p1_carts = carts;
	for (;; reset_updated(p1_carts)) {
		auto res = tick(map, p1_carts, false);
		if (res) {
			std::cout << "p1: " << std::get<0>(*res) << "," << std::get<1>(*res) << std::endl;
			break;
		}
	}

	auto p2_carts = carts;
	for (;; reset_updated(p2_carts)) {
		// p2 should never return a collision
		assert(!tick(map, p2_carts, true));

		int num_carts = 0;
		uint32_t solo_y = 0, solo_x = 0;

		for (uint32_t y = 0; y < p2_carts.size(); y++) {
			for (uint32_t x = 0; x < p2_carts[y].size(); x++) {
				if (p2_carts[y][x]) {
					num_carts++;
					solo_x = x;
					solo_y = y;
				}
			}
		}

		if (num_carts == 1) {
			std::cout << "p2: " << solo_x << "," << solo_y << std::endl;
			break;
		}
	}
	return 0;
}
