#include <getopt.h>
#include <string>
#include <stdexcept>
#include "day1.h"

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

    switch (day) {
    case 1: day1::run(); break;
    default: throw std::runtime_error("No such day");
    }

    return 0;
}
