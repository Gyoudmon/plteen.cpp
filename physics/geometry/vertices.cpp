#include "vertices.hpp"

#include "../mathematics.hpp"

using namespace Plteen;

/*************************************************************************************************/
Vertices Plteen::regular_polygon_vertices(size_t n, float radius, float rotation) {
    // for inscribed regular polygon, the radius should be `Rcos(pi/n)`
    float start = degrees_to_radians(rotation);
    float delta = 2.0F * pi_f / float(n);
    Vertices pvs;
    
    for (size_t idx = 0; idx < n; idx++) {
        pvs.push_back(std::polar(radius, start + delta * float(idx)));
    }

    return pvs;
}

Vertices Plteen::hexagon_tile_vertice(float width, float height) {
    if (width >= height) {
        float w25 = width  * 0.25F;
        float w75 = width  * 0.75F;
        float h50 = height * 0.50F;

        return { { w25, 0.0F },  { w75, 0.0F },  { width, h50},
                 { w75, height}, { w25, height}, { 0.0F,  h50} };
    } else {
        float h25 = height * 0.25F;
        float h75 = height * 0.75F;
        float w50 = width  * 0.50F;
        
        return { { w50, 0.0F },  { width, h25 }, { width, h75 },
                 { w50, height}, { 0.0F, h75},   { 0.0F,  h25} };
    }
}
