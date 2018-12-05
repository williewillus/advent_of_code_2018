#include "util.h"
#include <cctype>
#include <iterator>
#include <list>

namespace day5 {
static bool cancel(char a, char b) {
    return (std::isupper(a) && std::tolower(a) == b)
            || (std::islower(a) && std::toupper(a) == b);
}
    
static void run_one_iter(std::list<char>& input) {
    auto it = input.begin();
    auto next = std::next(it);
    while (next != input.end()) {
        if (cancel(*it, *next)) {
            auto nextit = std::next(next);
            input.erase(it);
            input.erase(next);
            
            it = nextit;
            if (it == input.end())
                break;
            next = std::next(it);
        } else {
            it = next;
            next++;
        }
    }
}

static void react(std::list<char>& input) {
    while (true) {
        auto size_before = input.size();
        run_one_iter(input);
        auto size_after = input.size();
        if (size_before == size_after) {
            return;
        }
    }
}
    
void run() {    
    std::list<char> input;
    std::string s = util::read_lines("d5_input.txt")[0];
    for (char c : s) {
        input.push_back(c);
    }
    
    {
        auto copy = input;
        react(copy);
        std::cout << "p1: " << copy.size() << std::endl;
    }
    
    unsigned int min_len = input.size();
    for (char to_remove : "abcdefghijklmnopqrstuvwxyz") {
        auto copy = input;
        char upper = std::toupper(to_remove);
        copy.remove_if([to_remove, upper](char cand) {
            return cand == to_remove || cand == upper;
        });
        react(copy);
        if (copy.size() < min_len) {
            min_len = copy.size();
        }
    }
    std::cout << "p2: " << min_len << std::endl;
}
}
