#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include "Settlement.hpp"
#include <fishnet/Polygon.hpp>
#include <fishnet/Vec2D.hpp>
#include <cmath>
#include <algorithm>  
#include <queue>         
#include <limits>
#include <functional> 

using namespace fishnet;
using ShapeType = fishnet::geometry::Polygon<double>;
using SettlementType = Settlement<ShapeType>;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;

//Compute the convex hull or alpha shape around the points of all polygons.
// Then compute the perimeter of that hull.

double clusterPerimeterFromHull(const std::vector<Point> hull) {

    // perimeter of hull
    double per = 0.0;
    for (size_t i = 0; i < hull.size(); ++i) {
        auto p1 = hull[i];                   
        auto p2 = hull[(i+1) % hull.size()]; 

        double dx = p2.x() - p1.x();         
        double dy = p2.y() - p1.y();         

        per += std::sqrt(dx*dx + dy*dy);     // Euclidean distance
    }
    return per;
}


double clusterAreaFromHull(const std::vector<Point>& hull) {
    if (hull.size() < 3) return 0.0; // no area for a line/point

    double area = 0.0;
    for (size_t i = 0; i < hull.size(); ++i) {
        const auto& p1 = hull[i];
        const auto& p2 = hull[(i+1) % hull.size()];
        area += (p1.x() * p2.y()) - (p2.x() * p1.y());
    }

    return std::abs(area) * 0.5;
}

// compactness of a single cluster 
double get_clusterCompactness(const std::vector<SettlementType>& cluster) {
    std::vector<ShapeType> clusterPolys;
    for(auto settlement : cluster){
        clusterPolys.push_back(settlement.getPolygon());
    }
    
    std::vector<Point> points;

    // collect all polygon points
    for (const auto& poly : clusterPolys) {
        for (const auto& pt : poly.getBoundary().getPoints()) {
            points.emplace_back(pt.x, pt.y);  
        }
    }

    std::vector<Point> hull;
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(hull));

    double perimeter = clusterPerimeterFromHull(hull);
    double area = clusterAreaFromHull(hull);

    double sum_pj = 0;
    double sum_area = 0;
    for(auto poly : cluster){
        sum_pj += poly.perimeter();
        sum_area += poly.getSize();
    }

    if (perimeter <= 0 || area <= 0) return 0.0;

    double CI_j = 2.0 * std::sqrt(area/ M_PI )/ perimeter; // circle perimeter
    return CI_j;
}


void compactnessIndex(const std::unordered_map<int,std::vector<SettlementType>>& clusters, fs::path file) {
    std::cout<< "compactness entered" << std::endl;
    std::ofstream out(file);
    double compactness_sum = 0;
    out << "Compactness values: \n";
    for(const auto& [cluster_id, cluster_settlements] : clusters) {
        double compactness = get_clusterCompactness(cluster_settlements);
        out << "Cluster " << cluster_id << " : compactness = " << compactness << "\n";
        compactness_sum += compactness;
    }

    double CI = compactness_sum/clusters.size();
    out << "Compactness Index (CI): " << CI << "\n";
    double normalized_CI = CI/clusters.size();
    out << "Normalized Compactness Index (CI): " << normalized_CI << "\n";

    out.close();
}

template <typename GraphT, typename PointType>
fishnet::graph::UndirectedGraph<PointType> build_from_nodes(const std::vector<PointType> &subset, GraphT graph) {
    auto subgraph = fishnet::graph::GraphFactory::UndirectedGraph<PointType>();
    // Add nodes
    subgraph.addNodes(subset);

    // For each node, check its neighbors and only keep edges inside the subset
    for (const auto &u : subset) {
        for (const auto &v : graph.getNeighbours(u)) {
            // if the edge uv or vu exists, add
            if(graph.containsEdge(u, v) || graph.containsEdge(v,u)){
                subgraph.addEdge(u,v);
            }
        }
    }
    return subgraph;
}

template <typename PointType>
int computeConnectivity(const fishnet::graph::UndirectedGraph<PointType> &g) {
    std::vector<PointType> nodes(g.getNodes().begin(), g.getNodes().end());
    if (nodes.empty()) return 0;
    int n = nodes.size();
    int minCut = n; // upper bound
 
    // Map PointType -> index
    std::unordered_map<PointType, int> nodeIndex;
    for (int i = 0; i < n; ++i) {
        nodeIndex[nodes[i]] = i;
    }
 
    // Iterate over all node pairs (s, t)
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int s = i;
            int t = j;
			// Build capacity matrix (adjacency-like)
            std::vector<std::vector<int>> capacity(n, std::vector<int>(n, 0));
            for (int u = 0; u < n; ++u) {
                for (auto &nbr : g.getNeighbours(nodes[u])) {
                    int v = nodeIndex[nbr];
                    capacity[u][v] += 1;
                    capacity[v][u] += 1; // undirected
                }
            }
 
            // Edmonds-Karp
            int maxFlow = 0;
            while (true) {
                std::vector<int> parent(n, -1);
                std::queue<std::pair<int,int>> q;
                q.push({s, std::numeric_limits<int>::max()});
                parent[s] = s;
                int flow = 0;
 
                while (!q.empty()) {
                    auto [cur, curFlow] = q.front(); q.pop();
                    if (cur == t) { flow = curFlow; break; }
					for (int v = 0; v < n; ++v) {
                        if (capacity[cur][v] > 0 && parent[v] == -1) {
                            parent[v] = cur;
                            q.push({v, std::min(curFlow, capacity[cur][v])});
                        }
                    }
                }
 
                if (flow == 0) break; // no more augmenting paths
                maxFlow += flow;
 
                // Update capacities
                int cur = t;
                while (cur != s) {
                    int p = parent[cur];
                    capacity[p][cur] -= flow;
                    capacity[cur][p] += flow;
                    cur = p;
                }
            }
 
            minCut = std::min(minCut, maxFlow);
        }
    }
 
    return minCut;
}

template <typename GraphT, typename PointType>
void get_connectivity(const std::unordered_map<int, std::vector<PointType>>& clusters, const GraphT& graph, const fs::path& file) {
    std::cout<< "connectivity entered" << std::endl;
    
    std::ofstream out(file);
    if (!out.is_open()) {
        std::cerr << "Failed to open file: " << file << std::endl;
        return;
    }
    
    out << "Connectivity values: \n";
    for (auto & [cluster_id, cluster_nodes] : clusters) {
        auto subgraph = build_from_nodes<GraphT, PointType>(cluster_nodes, graph); 
        int connectivity = computeConnectivity<PointType>(subgraph);
        out << "Cluster " << cluster_id << " : connectivity = " << connectivity << "\n";
    }
    out.close();
}




// function to determine a good eps for DBSCAN
std::vector<double> compute_k_distances(
    auto & graph,
    const std::unordered_map<SettlementType, std::unordered_map<SettlementType, double>>& distanceMap,
    int k)
{
    std::vector<double> kdistances;

    for (auto node : graph.getNodes()) {
        std::vector<double> distances;

        for (auto neighbor : graph.getNeighbours(node)) {
            distances.push_back(distanceMap.at(node).at(neighbor));
        }

        if (!distances.empty() && k <= static_cast<int>(distances.size())) {
            std::nth_element(distances.begin(), distances.begin() + (k - 1), distances.end());
            double kth_dist = distances[k - 1];
            kdistances.push_back(kth_dist);
        }
    }

    return kdistances;
}

// Compute percentile from a vector
double percentile(std::vector<double>& values, double percent)
{
    if (values.empty()) {
        return 0;
    }

    std::sort(values.begin(), values.end());
    double idx = (percent / 100.0) * (values.size() - 1);
    size_t lower = static_cast<size_t>(std::floor(idx));
    size_t upper = static_cast<size_t>(std::ceil(idx));
    double weight = idx - lower;

    return values[lower] * (1.0 - weight) + values[upper] * weight;
}

// Compute eps = 90th percentile of k-distances
double compute_eps_percentile(
    auto & graph,
    const std::unordered_map<SettlementType, std::unordered_map<SettlementType, double>>& distanceMap,
    int minPts,
    double per)
{
    int k = std::max(1, minPts - 1); // k-th nearest neighbor
    auto kdistances = compute_k_distances(graph, distanceMap, k);
    return percentile(kdistances, per);
}