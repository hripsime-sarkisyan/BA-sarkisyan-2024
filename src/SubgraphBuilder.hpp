#include <fishnet/Graph.hpp>
#include <fishnet/GraphFactory.hpp>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fishnet/Edge.hpp>




template <typename PointType, typename EdgeType>
struct SubgraphResult {
    std::vector<PointType> nodes;
    std::vector<EdgeType> edges;
};

template <typename PointType, typename GraphT, typename EdgeType>
class SubgraphBuilder {
public:
    SubgraphBuilder(const GraphT &original, int beta)
        : graph(original), beta(beta) {}

    SubgraphResult<PointType, EdgeType> build(const PointType &start) {
        SubgraphResult<PointType, EdgeType> result;
        visited.clear();
        dfs(start, 0, result.nodes, result.edges);
        return result;
    }

private:
    const GraphT &graph;
    int beta;
    std::unordered_set<PointType> visited;

    void dfs(const PointType &node, int depth,  std::vector<PointType> &nodes, std::vector<EdgeType> &edges) {
        if (depth > beta || visited.count(node)) return;
        visited.insert(node);

        // ensure node is added
        nodes.push_back(node);

        for (const auto &nbr : graph.getNeighbours(node)) {
            // add neighbor if within beta
            edges.emplace_back(node, nbr);
            nodes.push_back(nbr);

            dfs(nbr, depth + 1, nodes, edges);
        }
    }
};

template <typename PointType, typename GraphT, typename EdgeType>
SubgraphResult<PointType, EdgeType> beta_order_subgraph(
    const GraphT &graph,
    const PointType &start,
    int beta) 
{
    SubgraphBuilder<PointType, GraphT, EdgeType> builder(graph, beta);
    return builder.build(start);
}
