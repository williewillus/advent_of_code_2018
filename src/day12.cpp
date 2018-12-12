#include "util.h"
#include <array>
#include <numeric>
#include <regex>
#include <tuple>
namespace day12 {
static constexpr uint32_t PATTERN_SIZE = 5;
static constexpr char OCCUPIED = '#';
static constexpr char FREE = '.';

using pattern_antecedent = std::array<bool, PATTERN_SIZE>;
using pattern = std::tuple<pattern_antecedent, bool>;

static bool matches(const set<int>& prev_state, const pattern& p, int position) {
	const auto& antecedent = std::get<0>(p);
	return antecedent[0] == (prev_state.count(position) > 0)
		&& antecedent[1] == (prev_state.count(position + 1) > 0)
		&& antecedent[2] == (prev_state.count(position + 2) > 0)
		&& antecedent[3] == (prev_state.count(position + 3) > 0)
		&& antecedent[4] == (prev_state.count(position + 4) > 0);
}

static std::set<int> iter(const set<int>& prev_state, const vec<pattern>& patterns) {
	std::set<int> ret;
	int min = *prev_state.cbegin();
	int max = *prev_state.rbegin();
	int start = min - (PATTERN_SIZE - 1); // start with min on far right of window [....m]
	//std::cout << "max is " << max << std::endl;
	for (int i = start; i <= max; i++) {  // end with max on far left of window [m....]
		//std::cout << "trying " << i << std::endl;
		for (const auto& p : patterns) {
			if (matches(prev_state, p, i)) {
				//std::cout << "pattern matched at " << i << std::endl;
				// we match from the leftmost, but the consequent declares what happens to the 3rd pot
				bool consequent = std::get<1>(p);
				if (consequent)
					ret.insert(i+2);
				break;
			}
		}
	}

	return ret;
}

static void show(const set<int>& state) {
	for (int i : state) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;
}

void run() {
	auto lines = util::read_lines("d12_input.txt");

	auto initial_state_str = lines[0].substr(15); // strip off "initial state: "
	set<int> initial_state;
	for (uint32_t i = 0; i < initial_state_str.size(); i++) {
		if (initial_state_str[i] == OCCUPIED)
			initial_state.insert(i);
	}
	
	vec<pattern> patterns;
	for (uint32_t i = 2; i < lines.size(); i++) {
		const auto& line = lines[i];
		pattern_antecedent antecedent;
		for (uint32_t i = 0; i < PATTERN_SIZE; i++) {
			antecedent[i] = line[i] == OCCUPIED;
		}
		bool consequent = line[line.size()-1] == OCCUPIED;
		patterns.emplace_back(antecedent, consequent);
	}
	std::cout << "read " << patterns.size() << std::endl;

	auto p1_state = initial_state;
	show(p1_state);
	for (int i = 0; i < 20; i++) {
		auto next = iter(p1_state, patterns);
		p1_state = std::move(next);
		show(p1_state);
	}
	std::cout << "p1: " << std::accumulate(p1_state.cbegin(), p1_state.cend(), 0) << std::endl;

	auto p2_state = initial_state;
	for (uint64_t i = 0; i < 50000000000; i++) {
		if ((i % 1000) == 0)
			std::cout << "iter " << i << std::endl;
		auto next = iter(p2_state, patterns);
		p2_state = std::move(next);
	}
	std::cout << "p2: " << std::accumulate(p2_state.cbegin(), p2_state.cend(), 0) << std::endl;
}
}
