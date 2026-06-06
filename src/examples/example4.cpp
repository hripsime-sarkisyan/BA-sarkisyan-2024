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
#include "../DBSC.hpp"
#include "../helper.hpp"
#include "../EdgeVisualizer.hpp"
#include "../ConvertShapeFile.hpp"
#include "../GraphFactory.hpp"
#include "../WeightedNode.hpp"

using namespace std;
using namespace fishnet::geometry;
using namespace fishnet::util;
using namespace MyWorkflow;
namespace fs = std::filesystem;
using GraphT = fishnet::graph::UndirectedGraph<SimplePolygon<double>>;
int main() {

    auto graph = fishnet::graph::GraphFactory::UndirectedGraph<SimplePolygon<double>>();
    auto sizeAttributeExtractor = [](const SimplePolygon<double> & polygon){return polygon.area();};

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
    MyWorkflow::DBSC<SimplePolygon<double>, GraphT> dbsc1(1.5, 2);  // eps = 1.5, beta = 2
    static_assert(fishnet::util::UnaryFunction<decltype(sizeAttributeExtractor),SimplePolygon<double>, double> );
    auto clusters1 = dbsc1.run(graph, sizeAttributeExtractor); // void call,  beta=2
    //dbsc1.printGraph(graph);
    fs::path currentFile = __FILE__;
    fs::path projectRoot = currentFile.parent_path().parent_path();
    fs::path dbsc1output = projectRoot / "resources" / "dbsc1output.shp";

    convertShapeFile(clusters1, dbsc1output);

    return 0;
}