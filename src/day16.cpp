#include "util.h"
#include <array>
#include <sstream> 

namespace day16 {

enum opcode : uint8_t {
	ADDR,
	ADDI,
	MULR,
	MULI,
	BANR,
	BANI,
	BORR,
	BORI,
	SETR,
	SETI,
	GTIR,
	GTRI,
	GTRR,
	EQIR,
	EQRI,
	EQRR,
};

using state = std::array<uint32_t, 4>;
static state parse_state(const std::string& s) {
	state ret;
	char tmp;
	std::stringstream ss(s);

	ss >> tmp; // [
	ss >> ret[0];

	ss >> tmp; // ,
	ss >> ret[1];

	ss >> tmp; // ,
	ss >> ret[2];

	ss >> tmp; // ,
	ss >> ret[3];

	ss >> tmp; // ]

	return ret;
}

struct instruction {
	uint16_t op;
	uint32_t in1, in2, out;

	static instruction parse(const std::string& s) {
		instruction ret;
		std::stringstream ss(s);
		ss >> ret.op;
		ss >> ret.in1;
		ss >> ret.in2;
		ss >> ret.out;
		return ret;
	}
};

struct sample {
	instruction insn;
	state old_state, new_state;
};

static vec<opcode> test(const sample& s) {
	vec<opcode> ret;
	if (s.new_state[s.insn.out] == (s.old_state[s.insn.in1] + s.old_state[s.insn.in2]))
		ret.push_back(ADDR);
	if (s.new_state[s.insn.out] == (s.old_state[s.insn.in1] + s.insn.in2))
		ret.push_back(ADDI);
	if (s.new_state[s.insn.out] == (s.old_state[s.insn.in1] * s.old_state[s.insn.in2]))
		ret.push_back(MULR);
	if (s.new_state[s.insn.out] == (s.old_state[s.insn.in1] * s.insn.in2))
		ret.push_back(MULI);
	if (s.new_state[s.insn.out] == (s.old_state[s.insn.in1] & s.old_state[s.insn.in2]))
		ret.push_back(BANR);
	if (s.new_state[s.insn.out] == (s.old_state[s.insn.in1] & s.insn.in2))
		ret.push_back(BANI);
	if (s.new_state[s.insn.out] == (s.old_state[s.insn.in1] | s.old_state[s.insn.in2]))
		ret.push_back(BORR);
	if (s.new_state[s.insn.out] == (s.old_state[s.insn.in1] | s.insn.in2))
		ret.push_back(BORI);
	if (s.new_state[s.insn.out] == s.old_state[s.insn.in1])
		ret.push_back(SETR);
	if (s.new_state[s.insn.out] == s.insn.in1)
		ret.push_back(SETI);
	if (s.new_state[s.insn.out] == (s.insn.in1 > s.old_state[s.insn.in2]))
		ret.push_back(GTIR);
	if (s.new_state[s.insn.out] == (s.old_state[s.insn.in1] > s.insn.in2))
		ret.push_back(GTRI);
	if (s.new_state[s.insn.out] == (s.old_state[s.insn.in1] > s.old_state[s.insn.in2]))
		ret.push_back(GTRR);
	if (s.new_state[s.insn.out] == (s.insn.in1 == s.old_state[s.insn.in2]))
		ret.push_back(EQIR);
	if (s.new_state[s.insn.out] == (s.old_state[s.insn.in1] == s.insn.in2))
		ret.push_back(EQRI);
	if (s.new_state[s.insn.out] == (s.old_state[s.insn.in1] == s.old_state[s.insn.in2]))
		ret.push_back(EQRR);
	return ret;
}

static void show(const sample& s) {
	std::cout << "Before: " << s.old_state[0] << "," << s.old_state[1] << "," << s.old_state[2] << "," << s.old_state[3] << std::endl;
	std::cout << "insn: " << s.insn.op << "," << s.insn.in1 << "," << s.insn.in2 << "," << s.insn.out << std::endl;
	std::cout << "After: " << s.new_state[0] << "," << s.new_state[1] << "," << s.new_state[2] << "," << s.new_state[3] << std::endl;
}

void run() {
	vec<sample> samples;
	std::optional<sample> in_progress_sample;

	for (const auto& line : util::read_lines("d16_samples_input.txt")) { // todo won't work with input overrides since there's two files
		if (line.empty()) {
			continue;
		} else if (line.find("Before: ") != string::npos) {
			if (in_progress_sample)
				throw std::runtime_error("Trying to read new sample with one in progress");

			in_progress_sample = {sample()};
			in_progress_sample->old_state = parse_state(line.substr(8));	
		} else if (line.find("After: ") != string::npos) {
			if (!in_progress_sample)
				throw std::runtime_error("Trying to read after state with no sample in progress");

			in_progress_sample->new_state = parse_state(line.substr(7));	
			samples.push_back(*in_progress_sample);
			in_progress_sample = {};
		} else {
			if (!in_progress_sample)
				throw std::runtime_error("Trying to read sample insn with no sample in progress");
			in_progress_sample->insn = instruction::parse(line);
		}
	}

	show(samples[1]);
	uint32_t p1 = 0;
	for (const auto& s : samples) {
		auto t = test(s);
		if (t.size() >= 3)
			p1++;
	}
	std::cout << "p1: " << p1 << std::endl;
}
}
