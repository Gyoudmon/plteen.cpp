#pragma once

#include <stdexcept>

/*************************************************************************************************/
namespace Plteen {
    void raise_range_error(const char* message);
    void raise_range_error_if(size_t B, size_t b, const char* type);
}
