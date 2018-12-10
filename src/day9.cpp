#include "util.h"
#include <cassert>
#include <limits>
#include <regex>
#include <queue>

namespace day9 {
// TODO this solution causes reference cycles
struct Node {
	uint32_t value;
	std::shared_ptr<Node> prev;
	std::shared_ptr<Node> next;
};

static void insert_after(std::shared_ptr<Node>& left, uint32_t value) {
	auto right = left->next;
	auto node = std::make_shared<Node>();

	left->next = node;

	node->value = value;
	node->prev = left;
	node->next = right;

	right->prev = node;
}

// Returns new current node
static std::shared_ptr<Node> remove(std::shared_ptr<Node>& n) {
	auto left = n->prev;
	auto right = n->next;
	left->next = right;
	right->prev = left;
	return right;
}

static uint32_t run_one(uint32_t player_count, uint32_t last_marble) {
	uint32_t next_marble = 1;
	uint32_t cur_player = 0;

	vec<uint32_t> scores;
	scores.resize(player_count);

	std::shared_ptr<Node> cur_marble = std::make_shared<Node>();
	cur_marble->value = 0;
	cur_marble->prev = cur_marble;
	cur_marble->next = cur_marble;

	while (next_marble <= last_marble) {
		if (next_marble % 23 == 0) {
			scores.at(cur_player) += next_marble;
			auto left = cur_marble;
			for (int i = 0; i < 7; i++) {
				left = left->prev;
			}
			scores.at(cur_player) += left->value;
			cur_marble = remove(left);
		} else {
			insert_after(cur_marble->next, next_marble);
			cur_marble = cur_marble->next->next;
		}
		
		cur_player = (cur_player + 1) % player_count;
		next_marble++;
	}

	return *std::max_element(scores.cbegin(), scores.cend());
}

void run() {
	std::regex re("(\\d+) players; last marble is worth (\\d+) points");
	std::string input = util::read_lines("d9_input.txt")[0];
	std::smatch match;
	assert(std::regex_match(input, match, re));
	uint32_t player_count = std::stoul(match[1].str());
	uint32_t last_marble = std::stoul(match[2].str());

	std::cout << "p1: " << run_one(player_count, last_marble) << std::endl;
	std::cout << "p2: " << run_one(player_count, last_marble * 100) << std::endl;
}
}
