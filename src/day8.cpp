#include "util.h"
#include <cstdint>
#include <sstream>
#include <memory>
#include <numeric>
#include <optional>

namespace day8 {
struct Node {
	vec<std::shared_ptr<Node>> children;
	vec<uint32_t> metadata;
	std::optional<uint32_t> value;

	void parse(std::stringstream& s) {
		uint32_t num_children;
		uint32_t num_meta;
		s >> num_children;
		s >> num_meta;
		for (uint32_t i = 0; i < num_children; i++) {
			auto child = std::make_shared<Node>();
			child->parse(s);
			children.push_back(std::move(child));
		}
		for (uint32_t i = 0; i < num_meta; i++) {
			uint32_t tmp;
			s >> tmp;
			metadata.push_back(tmp);
		}
	}

	template<typename T>
	void traverse(T func) {
		func(*this);
		for (const auto& c : children) {
			c->traverse(func);
		}
	}

	uint32_t get_value() {
		if (!value) {
			if (children.empty()) {
				value = {std::accumulate(metadata.cbegin(), metadata.cend(), 0)};
			} else {
				uint32_t v = 0;
				for (uint32_t m : metadata) {
					uint32_t idx = m - 1;
					if (idx < children.size()) {
						v += children.at(idx)->get_value();
					}
				}
				value = {v};
			}
		}
		return *value;
	}
};

void run() {
	string data = util::read_lines("d8_input.txt")[0];
	std::stringstream s(data);
	Node root;
	root.parse(s);

	uint32_t meta_sum = 0;
	root.traverse([&meta_sum](const Node& n) {
		for (uint32_t meta : n.metadata) {
			meta_sum += meta;
		}
	});
	std::cout << "p1: " << meta_sum << std::endl;
	std::cout << "p2: " << root.get_value() << std::endl;
}
}
