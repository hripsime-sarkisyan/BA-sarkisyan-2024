#pragma once
#include <fishnet/Vec2D.hpp>
#include <fishnet/SimplePolygon.hpp>
#include <fstream>
#include <ogr_geometry.h>  // GDAL OGR headers
#include <fishnet/Vec2D.hpp>
#include <ranges>


std::unique_ptr<OGRPolygon> toOGRPolygon(const fishnet::geometry::SimplePolygon<double>& poly);


double euclideanDistance(const fishnet::geometry::Vec2D<double>& p1, const fishnet::geometry::Vec2D<double>& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

template<typename T>
void exportPolygonToSVG(const fishnet::geometry::SimplePolygon<T>& polygon,
    const std::string& filename = "/mnt/c/Users/hrips/Desktop/Uni WS24_25/Bachelor/Bachelor repo/2024-ba-hripsime-sarkisyan/resources/polygon.svg",
    int canvasWidth = 800, int canvasHeight = 600) {
        using Ring = fishnet::geometry::Ring<T>;
        using Vec2 = fishnet::geometry::Vec2D<T>;
        
        const Ring& ring = polygon.getBoundary();
        std::vector<Vec2> points;

        for (const auto& pt : ring.getPoints()) {
            points.push_back(pt);
        }

        if (points.empty()) return;

        auto [minX, maxX] = std::minmax_element(points.begin(), points.end(), 
        [](const Vec2& a, const Vec2& b) { return a.x < b.x; });
        auto [minY, maxY] = std::minmax_element(points.begin(), points.end(),
            [](const Vec2& a, const Vec2& b) { return a.y < b.y; });

        T xMin = minX->x;
        T xMax = maxX->x;
        T yMin = minY->y;
        T yMax = maxY->y;

        auto scaleX = [&](T x) {
            return int(((x - xMin) / (xMax - xMin + 1e-5)) * (canvasWidth - 20) + 10);
        };

        auto scaleY = [&](T y) {
            return int(((yMax - y) / (yMax - yMin + 1e-5)) * (canvasHeight - 20) + 10);
        };

        std::ofstream file(filename);
        file << "<svg xmlns='http://www.w3.org/2000/svg' width='" << canvasWidth
            << "' height='" << canvasHeight << "'>\n";

        // Draw polygon
        file << "<polygon points='";
        for (const auto& pt : points) {
            file << scaleX(pt.x) << "," << scaleY(pt.y) << " ";
        }
        file << "' fill='none' stroke='black' stroke-width='2'/>\n";

        // Optional: draw red dots at vertices
        for (const auto& pt : points) {
            file << "<circle cx='" << scaleX(pt.x) << "' cy='" << scaleY(pt.y)
                << "' r='3' fill='red'/>\n";
        }

        file << "</svg>\n";
        file.close();
}

template <typename P>
constexpr fishnet::geometry::Vec2D<double> compute_polygon_centroid(const std::vector<P> & points) noexcept {
    double area = 0.0;
    double cx = 0.0, cy = 0.0;
    const size_t n = points.size();

    for (size_t i= 0; i< n; i++) {
        const auto& p0 = points[i];
        const auto& p1 = points[(i +1)% n];
        double cross = p0.x() * p1.y() - p1.x() * p0.y();
        area += cross;
        cx += (p0.x() + p1.x()) * cross;
        cy += (p0.y() + p1.y()) * cross;
    }

    area *= 0.5;
    if (abs(area) < 1e-10) return {0.0, 0.0}; //degenerate case

    cx /= (6.0 * area);
    cy /= (6.0 * area);
    std::cout<< "centroid: (" << cx << "," << cy << ")" << std::endl;
    return {cx, cy};
}

template<typename Range>
auto to_vector(const Range& r) {
    return std::vector<std::ranges::range_value_t<Range>>(r.begin(), r.end());
}


std::string printPolygon(fishnet::geometry::SimplePolygon<double>& poly) {
    auto points = poly.getBoundary().getPoints();
    auto pointsVec = to_vector(points);
    std::string points_str = "";
    for (const auto& point : points) {
        points_str += point.toString() + ", ";
    }
    if (!points_str.empty()) {
        points_str.pop_back(); // remove last space
        points_str.pop_back(); // remove last comma
    }
    return points_str;
}