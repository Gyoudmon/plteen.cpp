#include "except.hpp"
#include "string.hpp"

using namespace Plteen;

/*************************************************************************************************/
void Plteen::raise_range_error(const char* message) {
    throw std::out_of_range(message);
}

void Plteen::raise_range_error_if(size_t B, size_t b, const char* type) {
    if (b >= B) {
        throw std::out_of_range(make_nstring("%s index too large, %zd >= %zd", type, b, B));
    }
}
