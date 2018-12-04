#include "util.h"
#include <array>
#include <cassert>
#include <ctime>
#include <regex>

namespace day4 {
enum Action {
    BEGIN_SHIFT,
    ASLEEP,
    WOKE
};

struct record {
    std::tm timestamp;
    Action action;
    // Only valid if action == begin_shift
    int guard_id;

    record(const std::tm& _time, Action _action, int _guard) : timestamp(_time), action(_action), guard_id(_guard) {}
};

struct guard {
    int id = 0;
    int total_sleep_time = 0;
    std::tm timestamp_fell_asleep {};
    std::array<int, 60> sleep_freqs = {};
};

static std::tm parse_time(const std::smatch& match) {
    std::tm t {};
    t.tm_year = std::stoi(match[1].str()) - 1900;
    t.tm_mon = std::stoi(match[2].str()) - 1;
    t.tm_mday = std::stoi(match[3].str());
    t.tm_hour = std::stoi(match[4].str());
    t.tm_min = std::stoi(match[5].str());
    return t;
}

static std::time_t mktime(const std::tm& tm) {
    std::tm copy = tm;
    auto ret = std::mktime(&copy);
    assert(ret != -1);
    return ret;
}

void run() {
    std::regex guard_re("\\[(\\d+)-(\\d+)-(\\d+) (\\d+):(\\d+)\\] Guard #(\\d+) .*");
    std::regex update_re("\\[(\\d+)-(\\d+)-(\\d+) (\\d+):(\\d+)\\] (.*)");
    vec<record> records;

    for (const auto& line : util::read_lines("d4_input.txt")) {
        std::smatch match;

        if (std::regex_match(line, match, guard_re)) {
            int guard = std::stoi(match[6].str());
            records.emplace_back(parse_time(match), BEGIN_SHIFT, guard);
        } else {
            assert(std::regex_match(line, match, update_re));
            std::string ack = match[6].str();
            if (ack == "wakes up") {
                records.emplace_back(parse_time(match), WOKE, -1);
            } else if (ack == "falls asleep") {
                records.emplace_back(parse_time(match), ASLEEP, -1);
            } else {
                throw std::runtime_error("INVALID ACTION");
            }
        }
    }

    std::sort(records.begin(), records.end(), [](const record& a, const record& b) {
        return mktime(a.timestamp) < mktime(b.timestamp);
    });

    map<int, guard> guards;
    int active_guard = -1;
    for (const auto& r: records) {
        switch (r.action) {
        case BEGIN_SHIFT:
            active_guard = r.guard_id;
            break;
        case ASLEEP: {
            auto& g = guards[active_guard];
            g.id = active_guard;
            g.timestamp_fell_asleep = r.timestamp;
            break;
        }
        case WOKE: {
            auto& g = guards[active_guard];

            auto time_asleep = std::difftime(mktime(r.timestamp), mktime(g.timestamp_fell_asleep)) + 1;
            g.total_sleep_time += time_asleep;

            for (int i = g.timestamp_fell_asleep.tm_min; i < r.timestamp.tm_min; i++) {
                g.sleep_freqs[i]++;
            }

            g.timestamp_fell_asleep = {};
            break;
        }
        }
    }

    {
        guard max_guard = std::max_element(guards.cbegin(), guards.cend(), [](const auto& e1, const auto& e2) {
            return e1.second.total_sleep_time < e2.second.total_sleep_time;
        })->second;

        auto max_sleep_freq = std::max_element(max_guard.sleep_freqs.cbegin(), max_guard.sleep_freqs.cend());
        auto max_freq_idx = std::distance(max_guard.sleep_freqs.cbegin(), max_sleep_freq);
        std::cout << "p1: " << max_guard.id * max_freq_idx << std::endl;
    }

    {
        guard max_guard = std::max_element(guards.cbegin(), guards.cend(), [](const auto& e1, const auto& e2) {
            auto max_sleep_freq_1 = std::max_element(e1.second.sleep_freqs.cbegin(), e1.second.sleep_freqs.cend());
            auto max_sleep_freq_2 = std::max_element(e2.second.sleep_freqs.cbegin(), e2.second.sleep_freqs.cend());
            return *max_sleep_freq_1 < *max_sleep_freq_2;
        })->second;

        auto max_sleep_freq = std::max_element(max_guard.sleep_freqs.cbegin(), max_guard.sleep_freqs.cend());
        auto max_freq_idx = std::distance(max_guard.sleep_freqs.cbegin(), max_sleep_freq);
        std::cout << "p2: " << max_guard.id * max_freq_idx << std::endl;
    }

}
}
