#include "util.h"

int main() {
    vec<long> changes = util::read_input<long>("d1_input.txt");

    long p1_counter = 0;
    for (long l : changes) {
        p1_counter += l;
    }
    std::cout << "p1: " << p1_counter << std::endl;

    set<long> seen;
    seen.insert(0);
    long p2_counter = 0;
    while (true) {
        for (long l : changes) {
            p2_counter += l;
            if (seen.find(p2_counter) == seen.end()) {
                seen.insert(p2_counter);
            } else {
                std::cout << "p2: " << p2_counter << std::endl;
                return 0;
            }
        }
    }
}
