#include "day1.h"
#include "util.h"

namespace day1 {

void run() {
    std::ifstream f("d1_input.txt");
    std::string in;
    std::vector<long> changes;

    long p1_counter = 0;
    while (std::getline(f, in)) {
        long l = std::stol(in);
        changes.push_back(l);
        p1_counter += l;
    }
    std::cout << "p1: " << p1_counter << std::endl;

    std::unordered_set<long> seen;
    seen.insert(0);
    long p2_counter = 0;
    while (true) {
        for (long l : changes) {
            p2_counter += l;
            if (seen.find(p2_counter) == seen.end()) {
                seen.insert(p2_counter);
            } else {
                std::cout << "p2: " << p2_counter << std::endl;
                return;
            }
        }
    }
}

}
