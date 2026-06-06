#include <fishnet/Graph.hpp>
#include <fishnet/GraphFactory.hpp>
#include <iostream>
#include <fishnet/Vec2D.hpp>
#include <fishnet/Contraction.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cmath>
#include "../DBSCAN.hpp"

using namespace std;
using namespace fishnet::geometry;
using namespace MyWorkflow;

int main() {
    
    vector<Vec2DStd> points = {{1, 1}, {2, 5}, {3, 3}, {4, 7}, {5, 2}, {6, 6}, {15, 15}, {16, 17}, {17, 14}, {18, 18}, {19, 15}, {20, 16}, 
    {10, 10}, {11, 11}, {12, 12}, {13, 13}};

    // Create the graph and add nodes
    auto graph = fishnet::graph::GraphFactory::UndirectedGraph<Vec2DStd>();
    graph.addNodes(points);

   // Add edges between normal points in Cluster 1
   graph.addEdge(Vec2DStd(1, 1), Vec2DStd(2, 5));
   graph.addEdge(Vec2DStd(2, 5), Vec2DStd(3, 3));
   graph.addEdge(Vec2DStd(3, 3), Vec2DStd(4, 7));
   graph.addEdge(Vec2DStd(4, 7), Vec2DStd(5, 2));
   graph.addEdge(Vec2DStd(5, 2), Vec2DStd(6, 6));

   // Add edges between normal points in Cluster 2
   graph.addEdge(Vec2DStd(15, 15), Vec2DStd(16, 17));
   graph.addEdge(Vec2DStd(16, 17), Vec2DStd(17, 14));
   graph.addEdge(Vec2DStd(17, 14), Vec2DStd(18, 18));
   graph.addEdge(Vec2DStd(18, 18), Vec2DStd(19, 15));
   graph.addEdge(Vec2DStd(19, 15), Vec2DStd(20, 16));

   // Add edges between the chain points (creating potential chain between clusters)
   graph.addEdge(Vec2DStd(10, 10), Vec2DStd(11, 11));
   graph.addEdge(Vec2DStd(11, 11), Vec2DStd(12, 12));
   graph.addEdge(Vec2DStd(12, 12), Vec2DStd(13, 13));


    // Test DBSCAN with different eps values
    cout << "Running DBSCAN with eps = 3, minPts = 2\n";
    DBSCAN<Vec2DStd, fishnet::graph::UndirectedGraph<Vec2DStd>> dbscan1(3.0, 2);
    dbscan1.run(graph);

    cout << "\nRunning DBSCAN with eps = 5, minPts = 2\n";
    DBSCAN<Vec2DStd, fishnet::graph::UndirectedGraph<Vec2DStd>> dbscan2(6.0, 2);
    dbscan2.run(graph);

    return 0;
}