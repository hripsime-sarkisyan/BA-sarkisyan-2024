#pragma once
#include <fishnet/SimplePolygon.hpp>
#include <fishnet/Ring.hpp>
//#include <fishnet/Printable.hpp>
//#include <fishnet/HashConcepts.hpp>
#include <fishnet/MemgraphClient.hpp>
#include <fishnet/MemgraphAdjacency.hpp>


template<typename PolygonT>
class Settlement{
private:
  size_t id;
  FileReference fileRef;
  PolygonT polygon;
    
  using polygon_type = PolygonT;
  //using numeric_type = typename PolygonT::numeric_type;

public:
    Settlement() = default;

    Settlement(size_t id, FileReference fileRef, PolygonT && polygon)
        : id(id), fileRef(std::move(fileRef)), polygon(std::move(polygon)) {}

    double getSize() const {
        return polygon.area();
    };

    PolygonT getPolygon() const {
        return polygon;
    };

    double distance(const Settlement<PolygonT>& other) const {
        return polygon.distance(other.polygon);
    };

    size_t key() const noexcept {
        return id;
    }

    const FileReference & file() const noexcept {
        return fileRef;
    }

    bool operator==(const Settlement<PolygonT>& other) const {
        return id == other.id;
    }

    Settlement(PolygonT geom) : polygon(std::move(geom)) {}

    const PolygonT& geometry() const { return polygon; }

    PolygonT& geometry() { return polygon; }

    std::string toString() const {
        return "Settlement(" + polygon.toString() + ")";
    }

    double perimeter() const {
        double perim = 0.0;
        for (const auto& seg : polygon.getBoundary().getSegments()) {
            perim += seg.length();
        }
        return perim;
    }
    
};

namespace std {
    template<typename PolygonT>
    struct hash<Settlement<PolygonT>> {
        size_t operator()(const Settlement<PolygonT>& settlement) const noexcept {
            // Hash the internal polygon
            const auto& poly = settlement.getPolygon();
            size_t polyHash = std::hash<PolygonT>{}(poly);
            return polyHash;
        }
    };
}

// namespace util {
//     template <typename PolygonT>
//     struct Printable<Settlement<PolygonT>> : std::true_type {
//         static void print(const Settlement<PolygonT>& s, std::ostream& os) {
//             os << "Settlement(";
//             Printable<PolygonT>::print(s.geometry(), os);
//             os << ")";
//         }
//     };

//     template <typename PolygonT>
//     struct Hashable<Settlement<PolygonT>> : std::true_type {
//         static size_t hash(const Settlement<PolygonT>& s) {
//             return Hashable<PolygonT>::hash(s.geometry());
//         }
//     };

// }