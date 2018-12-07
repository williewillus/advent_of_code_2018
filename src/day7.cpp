#include "util.h"
#include <array>
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

static string topsort(map<char, set<char>>& adj_list) {
	string result;
	std::priority_queue<char, std::vector<char>, std::greater<char>> queue;
	for (const auto& entry : adj_list) {
		auto node = entry.first;
		if (has_no_incoming_edges(adj_list, node))
			queue.push(node);
	}

	while (!queue.empty()) {
		char n = queue.top();
		queue.pop();
		result += n;
		auto nbs = std::move(adj_list[n]);
		adj_list[n].clear();
		for (char nb : nbs) {
			if (has_no_incoming_edges(adj_list, nb))
				queue.push(nb);
		}
	}

	return result;
}

using p2q = std::priority_queue<char, vec<char>, std::greater<char>>;
static void schedule_possible(map<char, set<char>>& dependencies, p2q& q) {
	vec<char> tmp;
	for (const auto& entry : dependencies) {
		if (entry.second.empty()) {
			tmp.push_back(entry.first);
		}
	}
	for (char c : tmp) {
		dependencies.erase(c);
		q.push(c);
	}
}

class worker {
private:
	static constexpr char IDLE = '.';
	char cur_task = IDLE;
	int time_left = 0;

	static int time_for_job(char n) {
		return 60 + int(n - 'A') + 1;
	}

	void on_finish(char task, map<char, set<char>>& dependencies, p2q& q) {
		std::cout << "---- completed task: " << task << std::endl;
		for (auto& entry : dependencies) {
			entry.second.erase(task);
		}
		schedule_possible(dependencies, q);
	}

	void get_new_task(p2q& q) {
		if (q.empty()) {
			std::cout << "---- queue empty" << std::endl;
			cur_task = IDLE;
		} else {
			cur_task = q.top();
			q.pop();
			time_left = time_for_job(cur_task);
			std::cout << "---- obtained task: " << cur_task << std::endl;
		}
	}

public:
	bool is_idle() const { return cur_task == IDLE; }

	void tick(map<char, set<char>>& dependencies, p2q& q) {
		if (is_idle()) {
			get_new_task(q);
		} else {
			time_left--;
			std::cout << "---- ticked task: " << cur_task << std::endl;
			if (time_left == 0) {
				on_finish(cur_task, dependencies, q);
				get_new_task(q);
			}
		}
	}
};

void run() {
	std::regex r("Step (\\w) must be finished before step (\\w) can begin.");
	map<char, set<char>> adj_list;
	map<char, set<char>> dependencies;
	for (const auto& s : util::read_lines("d7_input.txt")) {
		std::smatch match;
		assert(std::regex_match(s, match, r));
		auto from = match[1].str()[0];
		auto to = match[2].str()[0];

		adj_list[from].insert(to);
		// force empty set to be made for `to`
		adj_list[to];

		dependencies[to].insert(from);
		// force empty set to be made for `from`
		dependencies[from];
	}
	
	// run p2 first since it spams the log
	p2q q;
	schedule_possible(dependencies, q);

	std::array<worker, 5> workers;
	int timer = -1; // give one "pre-tick" to set things up
	while (true) {
		std::cout << "!!! beginning tick " << timer << std::endl;

		for (unsigned int i = 0; i < workers.size(); i++) {
			std::cout << "- worker " << i << std::endl;
			workers[i].tick(dependencies, q);
		}

		if (std::all_of(workers.cbegin(), workers.cend(), [](const auto& w) { return w.is_idle(); })) {
			if (!q.empty())
				std::cout << "!!!! ALL IDLING WITH NONEMPTY QUEUE" << std::endl;
			break;
		}
		timer++;
	}
	std::cout << "p2: " << timer << std::endl;

	std::cout << "p1: " << topsort(adj_list) << std::endl;;
}
}
