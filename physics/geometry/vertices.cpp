#include "vertices.hpp"

#include "../mathematics.hpp"

using namespace Plteen;

/*************************************************************************************************/
Plteen::Vertices Plteen::regular_polygon_vertices(size_t n, float radius, float rotation) {
    // for inscribed regular polygon, the radius should be `Rcos(pi/n)`
    float start = degrees_to_radians(rotation);
    float delta = 2.0F * pi_f / float(n);
    Vertices pvs;
    
    for (size_t idx = 0; idx < n; idx++) {
        pvs.push_back(std::polar(radius, start + delta * float(idx)));
    }

    return pvs;
}
