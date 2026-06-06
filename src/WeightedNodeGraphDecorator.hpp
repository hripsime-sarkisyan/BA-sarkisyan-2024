#pragma once
#include <fishnet/GraphDecorator.hpp>
#include <unordered_map>

namespace MyWorkflow {

/**
 * @brief Weighted node graph decorator
 * 
 * @tparam G base graph type (e.g. SimpleGraph)
 * @tparam A annotation type (e.g. double for weights)
 * @tparam W weight function type for nodes
 */
template<class G, Annotation A, WeightFunction<typename G::node_type,A> W>
class WeightedNodeGraphDecorator
    : public GraphDecorator<WeightedNodeGraphDecorator<G,A,W>, G, typename G::edge_type> {

private:
    using N = typename G::node_type;
    using E = typename G::edge_type;
    using Base = GraphDecorator<WeightedNodeGraphDecorator<G,A,W>, G, E>;
    using WeightMap = std::unordered_map<N, A>;

    WeightMap nodeWeights;  ///< stores node → weight mapping

public:
    using edge_type = typename G::edge_type;
    using adj_container_type = typename G::adj_container_type;

    WeightedNodeGraphDecorator() : Base() {}

    WeightedNodeGraphDecorator(adj_container_type&& adjContainer)
        : Base(std::move(adjContainer)) {}

    WeightedNodeGraphDecorator(util::forward_range_of<N> auto& nodes) : Base(nodes) {}

    WeightedNodeGraphDecorator(util::forward_range_of<N> auto&& nodes) : Base(nodes) {}

    // -------------------------------
    // Node handling with weights
    // -------------------------------
    bool addNode(const N& node, const A& weight) {
        if (Base::addNode(node)) {
            nodeWeights[node] = weight;
            return true;
        }
        return false;
    }

    bool addNode(N&& node, const A& weight) {
        if (Base::addNode(std::move(node))) {
            nodeWeights[node] = weight;
            return true;
        }
        return false;
    }

    /// Add node with default weight = A{}
    bool addNode(const N& node) {
        if (Base::addNode(node)) {
            nodeWeights[node] = A{};
            return true;
        }
        return false;
    }

    bool addNode(N&& node) {
        if (Base::addNode(std::move(node))) {
            nodeWeights[node] = A{};
            return true;
        }
        return false;
    }

    /// update weight
    void setWeight(const N& node, const A& weight) {
        nodeWeights[node] = weight;
    }

    bool hasWeight(const N& node) const {
        return nodeWeights.find(node) != nodeWeights.end();
    }

    /// get weight (throws if node not found)
    A getWeight(const N& node) const {
        auto it = nodeWeights.find(node);
        if (it == nodeWeights.end()) {
            throw std::runtime_error("Node weight not found");
        }
        return it->second;
    }

    /// get weight or default value if missing
    A getWeightOr(const N& node, const A& defaultValue) const {
        auto it = nodeWeights.find(node);
        return (it != nodeWeights.end()) ? it->second : defaultValue;
    }

        // Check node presence with optional weight match
    bool containsNode(const N& node) const {
        return Base::containsNode(node);
    }

    bool containsNode(const N& node, const A& weight) const {
        auto it = nodeWeights.find(node);
        return it != nodeWeights.end() && it->second == weight && Base::containsNode(node);
    }

    /// remove node (also its weight)
    void removeNode(const N& node) {
        nodeWeights.erase(node);
        Base::removeNode(node);
    }
    bool addEdge(const E& edge) {
        return Base::addEdge(edge);
    }

    // Bulk add edges
    void addEdges(util::forward_range_of<E> auto&& edges) {
        std::ranges::for_each(edges, [this](const E& e){ this->addEdge(e); });
    }

    // Add edge by endpoints
    bool addEdge(const N& from, const N& to) {
        return Base::addEdge(from, to);
    }

    bool addEdge(N&& from, N&& to) {
        return Base::addEdge(std::move(from), std::move(to));
    }

    // Contains edge
    bool containsEdge(const E& edge) const {
        return Base::containsEdge(edge);
    }

    bool containsEdge(const N& from, const N& to) const {
        return Base::containsEdge(from, to);
    }

    void removeEdge(const N& from, const N& to) {
        Base::removeEdge(from, to);
    }

    void removeEdge(const E& e) {
        Base::removeEdge(e);
    }
    inline auto getEdges() const {
        return this->g.getEdges();
    }

    inline auto getOutboundEdges(const N& node) const {
        return this->g.getOutboundEdges(node);
    }

    inline auto getInboundEdges(const N& node) const {
        return this->g.getInboundEdges(node);
    }


    // Clear graph + weights
    void clear() {
        nodeWeights.clear();
        Base::clear();
    }
};

}

