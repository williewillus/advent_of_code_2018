#include "util.h"
#include <algorithm>
#include <array>
#include <deque>
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

static state execute(instruction insn, const state& old) {
	state ret = old;
	switch (insn.op) {
	case ADDR: ret[insn.out] = old[insn.in1] + old[insn.in2]; break;
	case ADDI: ret[insn.out] = old[insn.in1] + insn.in2; break;
	case MULR: ret[insn.out] = old[insn.in1] * old[insn.in2]; break;
	case MULI: ret[insn.out] = old[insn.in1] * insn.in2; break;
	case BANR: ret[insn.out] = old[insn.in1] & old[insn.in2]; break;
	case BANI: ret[insn.out] = old[insn.in1] & insn.in2; break;
	case BORR: ret[insn.out] = old[insn.in1] | old[insn.in2]; break;
	case BORI: ret[insn.out] = old[insn.in1] | insn.in2; break;
	case SETR: ret[insn.out] = old[insn.in1]; break;
	case SETI: ret[insn.out] = insn.in1; break;
	case GTIR: ret[insn.out] = (insn.in1 > old[insn.in2]); break;
	case GTRI: ret[insn.out] = (old[insn.in1] > insn.in2); break;
	case GTRR: ret[insn.out] = (old[insn.in1] > old[insn.in2]); break;
	case EQIR: ret[insn.out] = (insn.in1 == old[insn.in2]); break;
	case EQRI: ret[insn.out] = (old[insn.in1] == insn.in2); break;
	case EQRR: ret[insn.out] = (old[insn.in1] == old[insn.in2]); break;
	}
	return ret;
}

struct sample {
	instruction insn;
	state old_state, new_state;
	vec<opcode> candidates;
};

static vec<opcode> get_candidates(const sample& s) {
	vec<opcode> ret;
	for (opcode o : {ADDR, ADDI, MULR, MULI, BANR, BANI, BORR, BORI, SETR, SETI, GTIR, GTRI, GTRR, EQIR, EQRI, EQRR}) {
		auto candidate = s.insn;
		candidate.op = o;
		if (execute(candidate, s.old_state) == s.new_state)
			ret.push_back(o);
	}
	return ret;
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
			in_progress_sample->candidates = get_candidates(*in_progress_sample);
			
			samples.push_back(std::move(*in_progress_sample));
			in_progress_sample = {};
		} else {
			if (!in_progress_sample)
				throw std::runtime_error("Trying to read sample insn with no sample in progress");
			in_progress_sample->insn = instruction::parse(line);
		}
	}

	uint32_t p1 = 0;
	for (const auto& s : samples) {
		if (s.candidates.size() >= 3)
			p1++;
	}
	std::cout << "p1: " << p1 << std::endl;

	map<uint16_t, opcode> mapping;
	std::deque<sample> queue;
	for (const auto& s : samples) {
		queue.push_back(s);
	}

	while (!queue.empty()) {
		// Take next sample
		auto next = std::move(queue.front());
		queue.pop_front();

		// If opcode of insn is already known, skip
		if (mapping.count(next.insn.op) > 0)
			continue;

		// Remove all known opcode from candidates
		for (const auto& p : mapping) {
			next.candidates.erase(
				std::remove(next.candidates.begin(), next.candidates.end(), p.second),
				next.candidates.end()
			);
		}

		// If one candidate opcode remains, we have a new mapping
		if (next.candidates.size() == 1) {
			mapping[next.insn.op] = next.candidates[0];
		}

		// Otherwise, put back in queue
		queue.push_back(std::move(next));
	}

	state p2state = {};
	for (const auto& s : util::read_lines("d16_program_input.txt")) {
		auto insn = instruction::parse(s);
		insn.op = mapping.at(insn.op);
		p2state = execute(insn, p2state);
	}
	std::cout << "p2: " << p2state[0] << std::endl;
}
}
