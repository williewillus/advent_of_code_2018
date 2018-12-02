#include <chrono>
#include <getopt.h>
#include <iostream>
#include <string>
#include <stdexcept>

namespace day1 { void run(); }
namespace day2 { void run(); }
namespace chrono = std::chrono;

int main(int argc, char* argv[]) {
    int day = -1;
    int ch = 0;
    while ((ch = getopt(argc, argv, "d:")) != -1) {
        if (ch == 'd') {
            day = std::stoi(optarg);
        } else {
            throw std::runtime_error("Argument error");
        }
    }

    if (day == -1) {
        throw std::runtime_error("Give day to run");
    }

    auto start = chrono::steady_clock::now();

    switch (day) {
    case 1: day1::run(); break;
    case 2: day2::run(); break;
    default: throw std::runtime_error("No such day");
    }

    auto delta = chrono::steady_clock::now() - start;
    std::cout << "Spent " << chrono::duration_cast<chrono::milliseconds>(delta).count() << " ms" << std::endl;

    return 0;
}
