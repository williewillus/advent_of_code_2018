#include "util.h"

static constexpr uint32_t INPUT = 846601;
static const string SEEKING = std::to_string(INPUT);

static void iter(vec<int>& elems, uint32_t& elf1_pos, uint32_t& elf2_pos) {
	auto elem1 = elems[elf1_pos];
	auto elem2 = elems[elf2_pos];
	auto sum = elem1 + elem2;
	if (sum >= 10) {
		elems.push_back(1); // will always be 1 since the max value of a single elem is 9
	}
	elems.push_back(sum % 10);
	elf1_pos = (elf1_pos + elem1 + 1) % elems.size();
	elf2_pos = (elf2_pos + elem2 + 1) % elems.size();
}

int main() {
	vec<int> elems { 3, 7 };
	uint32_t elf1_pos = 0;
	uint32_t elf2_pos = 1;
	std::optional<uint32_t> p2_answer;

	while (elems.size() < INPUT + 10 || !p2_answer) {
		iter(elems, elf1_pos, elf2_pos);

		if (elems.size() >= 5) {
			uint32_t base = elems.size() - 5;
			bool match = true;
			for (uint32_t i = 0; i < 5; i++) {
				auto there = elems[base+i];
				auto want = SEEKING[i] - '0';
				match = match && (there == want);
			}
			if (match) {
				p2_answer = {base};
			}
		}
	}	
	std::cout << "p1: ";
	for (uint32_t i = INPUT; i < INPUT+10; i++) {
		std::cout << elems[i];
	}
	std::cout << std::endl;
	std::cout << "p2: " << *p2_answer << std::endl;
	return 0;
}
