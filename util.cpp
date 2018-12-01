#include "util.h"

namespace util {
std::vector<long> read_as_ints(std::ifstream& in) {
    std::vector<long> ret;
    std::copy(std::istream_iterator<long>(in), {}, std::back_inserter(ret));
    return ret;
}
}
