// example graph with Polygons
#include <fishnet/Graph.hpp>
#include <fishnet/GraphFactory.hpp>
#include <iostream>
#include <fishnet/Vec2D.hpp>
#include <fishnet/Contraction.hpp>
#include <fishnet/SimplePolygon.hpp>
#include <fishnet/HashConcepts.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cmath>
#include "../DBSCAN.hpp"
#include "../helper.hpp"
#include "../EdgeVisualizer.hpp"
#include "../ConvertShapeFile.hpp"
#include <filesystem>

using namespace std;
using namespace fishnet::geometry;
using namespace fishnet::util;
using namespace MyWorkflow;
namespace fs = std::filesystem;
//using Polygon = fishnet::geometry::SimplePolygon<double>;


int main() {

    auto graph = fishnet::graph::GraphFactory::UndirectedGraph<SimplePolygon<double>>();

    SimplePolygon<double> poly1({
        Vec2D<double>(0, 0), Vec2D<double>(1, 0), Vec2D<double>(0.5, 1)
    });

    SimplePolygon<double> poly2({
        Vec2D<double>(0, 2), Vec2D<double>(2, 2), Vec2D<double>(3, 1), Vec2D<double>(2, 0)
    });

    SimplePolygon<double> poly3({
        Vec2D<double>(-1, -1), Vec2D<double>(1, -1), Vec2D<double>(-1, 0), Vec2D<double>(-2, 0)
    });

    SimplePolygon<double> poly4({
        Vec2D<double>(7, 4),Vec2D<double>(8, 4),Vec2D<double>(8.5, 5), Vec2D<double>(7.5, 6), Vec2D<double>(6.5, 5.5)
    });

    SimplePolygon<double> poly5({
        Vec2D<double>(6, 1), Vec2D<double>(5, 2), Vec2D<double>(6, 2)    
    });

    SimplePolygon<double> poly6({
        Vec2D<double>(-3, 1.5),
        Vec2D<double>(-2.5, 2.5),
        Vec2D<double>(-1.5, 2.8),
        Vec2D<double>(-1, 2),
        Vec2D<double>(-2, 1)    
    });

    SimplePolygon<double> poly7({
        Vec2D<double>(5, -1), Vec2D<double>(4, -1.5), Vec2D<double>(4, -2.5), Vec2D<double>(4.5, -3),Vec2D<double>(5.5, -2)     
    });

    std::cout << "7 SimplePolygon objects created successfully.\n" << poly1 << endl;
    vector<SimplePolygon<double>> points = {poly1, poly2, poly3, poly4, poly5, poly6, poly7};
    graph.addNodes(points);

    graph.addEdge(poly1, poly2);
    graph.addEdge(poly2, poly4);
    graph.addEdge(poly4, poly5);
    graph.addEdge(poly6, poly7);
    exportPolygonToSVG(poly1);
    //auto edge = visualizeEdge(poly1, poly2);
    // Apply DBSCAN with an epsilon that causes incorrect clustering due to the chain
    MyWorkflow::DBSCAN<SimplePolygon<double>, fishnet::graph::UndirectedGraph<SimplePolygon<double>>> dbscan1(1.5, 1);  // eps = 1.5, minPts = 1
    auto clusters1 = dbscan1.run(graph);

    fs::path currentFile = __FILE__;
    fs::path projectRoot = currentFile.parent_path().parent_path();
    fs::path output1 = projectRoot / "resources" / "output1.shp";

    convertShapeFile(clusters1, output1);

    // Fix the issue with different esp minPts values
    MyWorkflow::DBSCAN<SimplePolygon<double>, fishnet::graph::UndirectedGraph<SimplePolygon<double>>> dbscan2(2, 2);  // eps = 1, minPts = 1
    cout << "Fixed problem:" << endl;
    auto clusters2 = dbscan2.run(graph);

    fs::path output2 = projectRoot / "resources" / "output2.shp";
    
    convertShapeFile(clusters2, output2);
    return 0;
}