#include <fishnet/Graph.hpp>
#include <fishnet/GraphFactory.hpp>
#include <fishnet/SimplePolygon.hpp>
#include <fishnet/Vec2D.hpp>
#include <fishnet/HashConcepts.hpp>
#include <iostream>
#include <filesystem>
#include "../DBSCAN.hpp"
#include "../DBSC.hpp"
#include "../ConvertShapeFile.hpp"
#include "../EdgeVisualizer.hpp"
#include "../stats.hpp"
#include "../Settlement.hpp"

using namespace std;
using namespace MyWorkflow;
using namespace fishnet::geometry;
using namespace fishnet::util;
namespace fs = std::filesystem;
using GraphT = fishnet::graph::UndirectedGraph<SimplePolygon<double>>;
int main() {
    auto graph = GraphFactory::UndirectedGraph<SimplePolygon<double>>();

    // Attribute extractor: polygon area
    auto areaExtractor = [](const SimplePolygon<double> &poly){ return poly.area(); };

    // --- Construct polygons P1..P12 ---
    SimplePolygon<double> poly1({
        Vec2D<double>(1.300, 3.700),
        Vec2D<double>(2.300, 3.700),
        Vec2D<double>(2.300, 4.700),
        Vec2D<double>(1.300, 4.700)
    });

    SimplePolygon<double> poly2({
        Vec2D<double>(2.700, -0.900),
        Vec2D<double>(3.700, -0.900),
        Vec2D<double>(3.700, 0.100),
        Vec2D<double>(2.700, 0.100)
    });

    SimplePolygon<double> poly3({
        Vec2D<double>(2.093, 5.293),
        Vec2D<double>(3.507, 5.293),
        Vec2D<double>(3.507, 6.707),
        Vec2D<double>(2.093, 6.707)
    });

    SimplePolygon<double> poly4({
        Vec2D<double>(5.534, 3.334),
        Vec2D<double>(7.266, 3.334),
        Vec2D<double>(7.266, 5.066),
        Vec2D<double>(5.534, 5.066)
    });

    SimplePolygon<double> poly5({
        Vec2D<double>(7.600, 4.000),
        Vec2D<double>(9.600, 4.000),
        Vec2D<double>(9.600, 6.000),
        Vec2D<double>(7.600, 6.000)
    });

    SimplePolygon<double> poly6({
        Vec2D<double>(7.500, 7.500),
        Vec2D<double>(8.500, 7.500),
        Vec2D<double>(8.500, 8.500),
        Vec2D<double>(7.500, 8.500)
    });

    SimplePolygon<double> poly7({
        Vec2D<double>(6.364, 8.964),
        Vec2D<double>(10.836, 8.964),
        Vec2D<double>(10.836, 13.436),
        Vec2D<double>(6.364, 13.436)
    });

    SimplePolygon<double> poly8({
        Vec2D<double>(9.882, 8.282),
        Vec2D<double>(12.118, 8.282),
        Vec2D<double>(12.118, 10.518),
        Vec2D<double>(9.882, 10.518)
    });

    SimplePolygon<double> poly9({
        Vec2D<double>(12.775, 3.775),
        Vec2D<double>(15.225, 3.775),
        Vec2D<double>(15.225, 6.225),
        Vec2D<double>(12.775, 6.225)
    });

    SimplePolygon<double> poly10({
        Vec2D<double>(15.463, 5.263),
        Vec2D<double>(16.937, 5.263),
        Vec2D<double>(16.937, 6.737),
        Vec2D<double>(15.463, 6.737)
    });

    SimplePolygon<double> poly11({
        Vec2D<double>(16.342, 8.342),
        Vec2D<double>(18.058, 8.342),
        Vec2D<double>(18.058, 10.058),
        Vec2D<double>(16.342, 10.058)
    });

    SimplePolygon<double> poly12({
        Vec2D<double>(18.299, 5.699),
        Vec2D<double>(21.301, 5.699),
        Vec2D<double>(21.301, 8.701),
        Vec2D<double>(18.299, 8.701)
    });

    SimplePolygon<double> poly13({
        Vec2D<double>(1.500, 4.300),
        Vec2D<double>(2.500, 4.300),
        Vec2D<double>(2.500, 5.300),
        Vec2D<double>(1.500, 5.300)
    });

    SimplePolygon<double> poly14({
        Vec2D<double>(3.893, 6.893),
        Vec2D<double>(5.307, 6.893),
        Vec2D<double>(5.307, 8.307),
        Vec2D<double>(3.893, 8.307)
    });

    SimplePolygon<double> poly15({
        Vec2D<double>(3.700, 2.500),
        Vec2D<double>(4.700, 2.500),
        Vec2D<double>(4.700, 3.500),
        Vec2D<double>(3.700, 3.500)
    });

    SimplePolygon<double> poly16({
        Vec2D<double>(5.534, 0.134),
        Vec2D<double>(6.866, 0.134),
        Vec2D<double>(6.866, 1.466),
        Vec2D<double>(5.534, 1.466)
    });

    SimplePolygon<double> poly17({
        Vec2D<double>(7.600, 0.400),
        Vec2D<double>(9.600, 0.400),
        Vec2D<double>(9.600, 2.400),
        Vec2D<double>(7.600, 2.400)
    });

    SimplePolygon<double> poly18({
        Vec2D<double>(9.600, 6.600),
        Vec2D<double>(12.400, 6.600),
        Vec2D<double>(12.400, 9.400),
        Vec2D<double>(9.600, 9.400)
    });

    SimplePolygon<double> poly19({
        Vec2D<double>(10.882, 9.882),
        Vec2D<double>(13.118, 9.882),
        Vec2D<double>(13.118, 12.118),
        Vec2D<double>(10.882, 12.118)
    });

    SimplePolygon<double> poly20({
        Vec2D<double>(14.299, 11.099),
        Vec2D<double>(17.301, 11.099),
        Vec2D<double>(17.301, 14.101),
        Vec2D<double>(14.299, 14.101)
    });

    SimplePolygon<double> poly21({
        Vec2D<double>(10.975, 0.575),
        Vec2D<double>(13.425, 0.575),
        Vec2D<double>(13.425, 3.025),
        Vec2D<double>(10.975, 3.025)
    });

    SimplePolygon<double> poly22({
        Vec2D<double>(14.183, 0.983),
        Vec2D<double>(15.817, 0.983),
        Vec2D<double>(15.817, 2.617),
        Vec2D<double>(14.183, 2.617)
    });

    SimplePolygon<double> poly23({
        Vec2D<double>(12.775, 6.775),
        Vec2D<double>(15.225, 6.775),
        Vec2D<double>(15.225, 9.225),
        Vec2D<double>(12.775, 9.225)
    });

    SimplePolygon<double> poly24({
        Vec2D<double>(15.134, 2.534),
        Vec2D<double>(16.866, 2.534),
        Vec2D<double>(16.866, 4.266),
        Vec2D<double>(15.134, 4.266)
    });

    SimplePolygon<double> poly25({
        Vec2D<double>(17.517, 4.517),
        Vec2D<double>(19.283, 4.517),
        Vec2D<double>(19.283, 6.283),
        Vec2D<double>(17.517, 6.283)
    });

    SimplePolygon<double> poly26({
        Vec2D<double>(18.418, 10.618),
        Vec2D<double>(21.582, 10.618),
        Vec2D<double>(21.582, 13.782),
        Vec2D<double>(18.418, 13.782)
    });

    SimplePolygon<double> poly27({
        Vec2D<double>(18.299, 8.299),
        Vec2D<double>(20.501, 8.299),
        Vec2D<double>(20.501, 10.501),
        Vec2D<double>(18.299, 10.501)
    });

    SimplePolygon<double> poly28({
        Vec2D<double>(20.418, 7.218),
        Vec2D<double>(23.582, 7.218),
        Vec2D<double>(23.582, 10.382),
        Vec2D<double>(20.418, 10.382)
    });

    SimplePolygon<double> poly29({
        Vec2D<double>(13.677, 8.877),
        Vec2D<double>(16.323, 8.877),
        Vec2D<double>(16.323, 11.523),
        Vec2D<double>(13.677, 11.523)
    });

    vector<SimplePolygon<double>> nodes = {poly1,poly2,poly3,poly4,poly5,poly6,poly7,poly8,poly9,poly10,poly11,poly12,poly13, 
        poly14,poly15,poly16,poly17,poly18,poly19,poly20,poly21,poly22,poly23,poly24,poly25,poly26,poly27,poly28, poly29};
    
    graph.addNodes(nodes);

    

    // --- Edges from Fig.3 ---
    graph.addEdge(poly1, poly13);
    graph.addEdge(poly1, poly3);
    graph.addEdge(poly1, poly2);
    graph.addEdge(poly1, poly15);
    graph.addEdge(poly2, poly16);
    graph.addEdge(poly2, poly15);
    graph.addEdge(poly15, poly16);
    graph.addEdge(poly15, poly3);
    graph.addEdge(poly15, poly4);
    graph.addEdge(poly3, poly13);
    graph.addEdge(poly3, poly14);
    graph.addEdge(poly3, poly4);
    graph.addEdge(poly14, poly6);
    graph.addEdge(poly14, poly13);
    graph.addEdge(poly14, poly4);
    graph.addEdge(poly4, poly16);
    graph.addEdge(poly4, poly17);
    graph.addEdge(poly4, poly5);
    graph.addEdge(poly4, poly6);
    graph.addEdge(poly6, poly18);
    graph.addEdge(poly6, poly7);
    graph.addEdge(poly6, poly5);
    graph.addEdge(poly5, poly8);
    graph.addEdge(poly5, poly17);
    graph.addEdge(poly5, poly18);
    graph.addEdge(poly17, poly16);
    graph.addEdge(poly17, poly21);
    graph.addEdge(poly17, poly8);
    graph.addEdge(poly7, poly19);
    graph.addEdge(poly7, poly18);
    graph.addEdge(poly18, poly19);
    graph.addEdge(poly18, poly23);
    graph.addEdge(poly18, poly8);
    graph.addEdge(poly18, poly9);
    graph.addEdge(poly8, poly21);
    graph.addEdge(poly8, poly9);
    graph.addEdge(poly19, poly20);
    graph.addEdge(poly19, poly29);
    graph.addEdge(poly19, poly23);
    graph.addEdge(poly9, poly10);
    graph.addEdge(poly9, poly24);
    graph.addEdge(poly9, poly22);
    graph.addEdge(poly9, poly21);
    graph.addEdge(poly9, poly23);
    graph.addEdge(poly20, poly11);
    graph.addEdge(poly20, poly29);
    graph.addEdge(poly20, poly26);
    graph.addEdge(poly24, poly25);
    graph.addEdge(poly24, poly10);
    graph.addEdge(poly10, poly23);
    graph.addEdge(poly10, poly11);
    graph.addEdge(poly10, poly12);
    graph.addEdge(poly10, poly25);
    graph.addEdge(poly25, poly12);
    graph.addEdge(poly25, poly28);
    graph.addEdge(poly11, poly29);
    graph.addEdge(poly11, poly26);
    graph.addEdge(poly11, poly27);
    graph.addEdge(poly11, poly23);
    graph.addEdge(poly11, poly12);
    graph.addEdge(poly12, poly27);
    graph.addEdge(poly12, poly28);
    graph.addEdge(poly26, poly27);

    cout << "Graph with 29 polygons constructed.\n";


    // visualize edge
    for(auto edge : graph.getEdges()){
        visualizeEdge(edge.getFrom(), edge.getTo());
    }

    // --- Run DBSC ---
    MyWorkflow::DBSC<SimplePolygon<double>, GraphT> dbsc(2);   // (T1 is set to 1.9), beta is 2
    //static_assert(fishnet::util::UnaryFunction<decltype(areaExtractor), SimplePolygon<double>, double>);
    auto clusters = dbsc.run(graph, areaExtractor);  // beta=2 (example)

    // --- Export result to shapefile ---
    fs::path currentFile = __FILE__;
    fs::path projectRoot = currentFile.parent_path().parent_path().parent_path();
    fs::path output = projectRoot / "resources" / "fin1_dbsc_example6_output.shp";
    convertShapeFile(clusters, output);

    fs::path conn_output = projectRoot / "stats"/ "fin1_dbsc_example6_connectivity.txt";
    get_connectivity<GraphT, SimplePolygon<double>>(clusters, graph, conn_output);
    return 0;
}
