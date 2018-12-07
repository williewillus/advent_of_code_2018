#include "util.h"
#include <cassert>
#include <list>
#include <regex>
#include <queue>

namespace day7 {

static bool has_no_incoming_edges(const map<char, set<char>>& adj_list, char node) {
	for (const auto& entry : adj_list) {
		if (entry.second.count(node) > 0) {
			return false;
		}
	}
	return true;
}

static vec<char> topsort(map<char, set<char>>& adj_list) {
	vec<char> result;
	std::priority_queue<char, std::vector<char>, std::greater<char>> queue;
	for (const auto& entry : adj_list) {
		auto node = entry.first;
		if (has_no_incoming_edges(adj_list, node))
			queue.push(node);
	}

	while (!queue.empty()) {
		char n = queue.top();
		queue.pop();
		result.push_back(n);
		auto nbs = std::move(adj_list[n]);
		adj_list[n].clear();
		for (char nb : nbs) {
			if (has_no_incoming_edges(adj_list, nb))
				queue.push(nb);
		}
	}

	return result;
}

void run() {
	std::regex r("Step (\\w) must be finished before step (\\w) can begin.");
	map<char, set<char>> adj_list;
	for (const auto& s : util::read_lines("d7_input.txt")) {
		std::smatch match;
		assert(std::regex_match(s, match, r));
		auto from = match[1].str()[0];
		auto to = match[2].str()[0];
		adj_list[from].insert(to);
		// force empty set to be made for `to`
		adj_list[to];
	}

	auto ts = topsort(adj_list);

	std::cout << "p1: ";
	for (char c : ts) {
		std::cout << c;
	}
	std::cout << std::endl;

}
}
