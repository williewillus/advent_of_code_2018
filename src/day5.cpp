#include "util.h"
#include <algorithm>
#include <cctype>
#include <deque>

static bool cancel(char a, char b) {
    return (std::isupper(a) && std::tolower(a) == b)
            || (std::islower(a) && std::toupper(a) == b);
}

static unsigned int reacted_size(const std::string& input) {
    std::deque<char> stack;
    for (char c : input) {
	if (stack.empty() || !cancel(c, stack.back())) {
	    stack.push_back(c);
	} else {
	    stack.pop_back();
	}
    }
    return stack.size();
}
    
int main() {    
    std::string s = util::read_lines("d5_input.txt")[0];
    std::cout << "p1: " << reacted_size(s) << std::endl;
    
    unsigned int min_len = s.size();
    for (char to_remove : "abcdefghijklmnopqrstuvwxyz") {
        char to_remove_upper = std::toupper(to_remove);
        auto copy = s;
		copy.erase(
			std::remove_if(copy.begin(), copy.end(), [to_remove, to_remove_upper](char cand) {
				return cand == to_remove || cand == to_remove_upper;
			}),
		copy.cend());
		auto rs = reacted_size(copy);
        if (rs < min_len) {
            min_len = rs;
        }
    }
    std::cout << "p2: " << min_len << std::endl;
	return 0;
}
