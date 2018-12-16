#include "util.h"
#include <memory>

namespace day15 {

struct grid_elem {
	virtual bool is_free() { return true; }
	virtual bool act() { return true; }
	virtual void test() {}
};

struct wall : public grid_elem {
	bool is_free() override { return false; }
};

struct goblin : public grid_elem {
	bool is_free() override { return false; }
	bool act() override {
		return true;
	}
};

struct elf : public grid_elem {
	bool is_free() override { return false; }
	bool act() override {
		return true;
	}
	void test() override { std::cout << "test" << std::endl; }
};

void run() {
	vec<vec<std::unique_ptr<grid_elem>>> world;
	auto lines = util::read_lines("d15_input.txt");
	for (const auto& line : lines) {
		vec<std::unique_ptr<grid_elem>> row;
		for (char c : line) {
			switch (c) {
			case '#': row.push_back(std::make_unique<wall>()); break;
			case '.': row.push_back(std::make_unique<grid_elem>()); break;
			case 'G': row.push_back(std::make_unique<goblin>()); break;
			case 'E': row.push_back(std::make_unique<elf>()); break;
			}
		}
		world.push_back(std::move(row));
	}
}

}
