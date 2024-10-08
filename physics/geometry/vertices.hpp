#pragma once

#include <vector>

#include "../algebra/point.hpp"

namespace Plteen {
    typedef std::vector<Plteen::Dot> polygon_vertices;

    __lambda__ Plteen::polygon_vertices regular_polygon_vertices(size_t n, float radius, float rotation);
}
