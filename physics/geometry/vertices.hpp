#pragma once

#include <vector>
#include <complex>

namespace Plteen {
    typedef std::vector<std::complex<float>> Vertices;

    __lambda__ Plteen::Vertices regular_polygon_vertices(size_t n, float radius, float rotation);
}
