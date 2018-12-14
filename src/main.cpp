#include <chrono>
#include <getopt.h>
#include <iostream>
#include <string>
#include <stdexcept>

namespace day1 { void run(); }
namespace day2 { void run(); }
namespace day3 { void run(); }
namespace day4 { void run(); }
namespace day5 { void run(); }
namespace day6 { void run(); }
namespace day7 { void run(); }
namespace day8 { void run(); }
namespace day9 { void run(); }
namespace day10 { void run(); }
namespace day11 { void run(); }
namespace day12 { void run(); }
namespace day13 { void run(); }
namespace day14 { void run(); }
namespace util { extern std::string input_override; }
namespace chrono = std::chrono;

int main(int argc, char* argv[]) {
    int day = -1;
    int ch = 0;
    while ((ch = getopt(argc, argv, "o:d:")) != -1) {
	switch (ch) {
          case 'd': day = std::stoi(optarg); break;
	  case 'o': util::input_override = optarg; break;
        }
    }

    if (day == -1) {
        throw std::runtime_error("Give day to run");
    }

    auto start = chrono::steady_clock::now();

    switch (day) {
    case 1: day1::run(); break;
    case 2: day2::run(); break;
    case 3: day3::run(); break;
    case 4: day4::run(); break;
    case 5: day5::run(); break;
    case 6: day6::run(); break;
    case 7: day7::run(); break;
    case 8: day8::run(); break;
    case 9: day9::run(); break;
    case 10: day10::run(); break;
    case 11: day11::run(); break;
    case 12: day12::run(); break;
    case 13: day13::run(); break;
    case 14: day14::run(); break;
    default: throw std::runtime_error("No such day");
    }

    auto delta = chrono::steady_clock::now() - start;
    std::cout << "Spent " << chrono::duration_cast<chrono::milliseconds>(delta).count() << " ms" << std::endl;

    return 0;
}
