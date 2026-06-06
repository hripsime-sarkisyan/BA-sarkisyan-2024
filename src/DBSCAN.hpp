#include <fishnet/Graph.hpp>
#include <fishnet/GraphFactory.hpp>
#include <iostream>
#include <fishnet/Vec2D.hpp>
#include <fishnet/Contraction.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cmath>


namespace MyWorkflow{

template <typename PointType, typename GraphT>
class DBSCAN {
    public:
    DBSCAN(double eps, int minPts) : eps(eps), minPts(minPts) {}

    std::unordered_map<int, std::vector<PointType>> run(GraphT & graph) {
        std::cout << "dbscan entered" << std::endl;
        int clusterID = 0;
        std::unordered_map<PointType, int> labels;
        int nodeentered = 0;
        for (const auto& node : graph.getNodes()) {
            std::cout<< "nodeentered" << nodeentered << std::endl;
            if (labels.find(node) != labels.end()) {
                continue;  // Already visited
            }

            std::vector<PointType> neighbors = regionQuery(graph, node);
            if (neighbors.size() < minPts) {
                labels[node] = -1;  // Mark as noise
            } else {
                ++clusterID;
                expandCluster(graph, node, neighbors, clusterID, labels);
            }
            nodeentered++;
        }

        // return a list of clutsers as a result
        return extractClusters(labels);
        // Output
        // printClusters(labels);
    }
    private:
    double eps;
    int minPts;

    std::vector<PointType> regionQuery(GraphT& graph, const PointType & point) {
        std::vector<PointType> neighbors;
        for (const auto & neighbor : graph.getNeighbours(point)) {
            if (point.distance(neighbor) <= eps) {
                neighbors.push_back(neighbor);
            }
        }
        return neighbors;
    }

    void expandCluster(GraphT& graph, const PointType& point,
                       std::vector<PointType>& neighbors, int clusterID,
                       std::unordered_map<PointType, int>& labels) {
        std::queue<PointType> toProcess;
        labels[point] = clusterID;

        for (const auto& neighbor : neighbors) {
            toProcess.push(neighbor);
        }

        while (!toProcess.empty()) {
            PointType current = toProcess.front();
            toProcess.pop();

            if (labels.find(current) == labels.end()) {
                labels[current] = clusterID;

                std::vector<PointType> newNeighbors = regionQuery(graph, current);
                if (newNeighbors.size() >= minPts) {
                    for (const auto& newNeighbor : newNeighbors) {
                        if (labels.find(newNeighbor) == labels.end()) {
                            toProcess.push(newNeighbor);
                        }
                    }
                }
            }
        }
    }
     std::unordered_map<int, std::vector<PointType>>  extractClusters (const std::unordered_map<PointType, int> & labels) {
        std::unordered_map<int, std::vector<PointType>> clusters;
        for (const auto& pair : labels ) {
            clusters[pair.second].push_back(pair.first);
        }

        // std::vector<std::vector<PointType>> result;
        // for (const auto& cluster : clusters){
        //     result.push_back(cluster.second);
        // }
        return clusters;
    }

    void printClusters(const std::unordered_map<PointType, int>& labels) {
        std::unordered_map<int, std::vector<PointType>> clusters;
        for (const auto& pair : labels) {
            clusters[pair.second].push_back(pair.first);
        }

        //for (const auto& cluster : clusters) {
        //    cout<< "Cluster " << cluster.first << ": ";
        //    for (const auto& point : cluster.second) {
        //        cout<< "(" << point.x << "," << point.y << ") ";
        //    }
        //    cout<< "\n";
        //}
    }

};


}




