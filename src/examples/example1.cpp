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

    // Create an undirected graph
    auto graph = fishnet::graph::GraphFactory::UndirectedGraph<Vec2DStd>();

    // Define nodes forming two clusters connected by a sparse "chain"
    vector<Vec2DStd> nodes = {
        {1, 1}, {2, 2}, {3, 3},   // Cluster 1
        {5, 5}, {6, 6}, {7, 7},   // Cluster 2
        {3.5, 3.5}, {4, 4}, {4.5, 4.5}  // Chain nodes connecting the clusters
    };

    // Add nodes to the graph
    graph.addNodes(nodes);

    // Add intra-cluster edges (strong connectivity within clusters)
    graph.addEdge(Vec2DStd(1, 1), Vec2DStd(2, 2));
    graph.addEdge(Vec2DStd(2, 2), Vec2DStd(3, 3));

    graph.addEdge(Vec2DStd(5, 5), Vec2DStd(6, 6));
    graph.addEdge(Vec2DStd(6, 6), Vec2DStd(7, 7));

    // Add chain edges (weak links that create the "settlement chain" problem)
    graph.addEdge(Vec2DStd(3, 3), Vec2DStd(3.5, 3.5));
    graph.addEdge(Vec2DStd(3.5, 3.5), Vec2DStd(4, 4));
    graph.addEdge(Vec2DStd(4, 4), Vec2DStd(4.5, 4.5));
    graph.addEdge(Vec2DStd(4.5, 4.5), Vec2DStd(5, 5));

    // Print edges
    cout << "Graph edges:" << endl;
    for (auto &&edge : graph.getEdges()) {
        cout << "Edge from (" << edge.getFrom().x << ", " << edge.getFrom().y << ") to ("
             << edge.getTo().x << ", " << edge.getTo().y << ") Distance: " 
             << edge.getFrom().distance(edge.getTo()) << endl;
    }

    // Apply DBSCAN with an epsilon that causes incorrect clustering due to the chain
    MyWorkflow::DBSCAN<Vec2DStd, fishnet::graph::UndirectedGraph<Vec2DStd>> dbscan1(1.5, 1);  // eps = 1.5, minPts = 1
    dbscan1.run(graph);


    // Fix the issue with different esp minPts values
    MyWorkflow::DBSCAN<Vec2DStd, fishnet::graph::UndirectedGraph<Vec2DStd>> dbscan2(1, 1);  // eps = 1, minPts = 1
    cout << "Fixed problem:" << endl;
    dbscan2.run(graph);

    return 0;
}
