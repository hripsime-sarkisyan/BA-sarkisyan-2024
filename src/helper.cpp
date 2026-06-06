#include "helper.hpp"

// Helper conversion function: converts SimplePolygon<double> -> OGRPolygon*
std::unique_ptr<OGRPolygon> fishnet::toOGRPolygon(const geometry::SimplePolygon<double>& poly) {
    auto ring = std::make_unique<OGRLinearRing>();
    for (const auto& pt : poly.getPoints()) {
        ring->addPoint(pt.x, pt.y);
    }
    ring->closeRings();

    auto ogrPoly = std::make_unique<OGRPolygon>();
    ogrPoly->addRingDirectly(ring.release());

    return ogrPoly;
}