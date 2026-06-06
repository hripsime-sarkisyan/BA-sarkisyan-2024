#pragma once
#include "WeightedNodeGraphDecorator.hpp"

namespace MyWorkflow {

/**
 * @brief Concrete WeightedNodeGraph class
 * 
 * Wraps WeightedNodeGraphDecorator so it can be directly instantiated.
 *
 * @tparam G base graph type
 * @tparam A annotation type (e.g. double)
 * @tparam W weight function type
 */
template<class G, typename A, typename W>
class WeightedNodeGraph : public WeightedNodeGraphDecorator<G,A,W> {
    using Base = WeightedNodeGraphDecorator<G,A,W>;

public:
    using N = typename G::node_type;
    using E = typename G::edge_type;
    using Base::addNode;
    using Base::addEdge;
    using Base::setWeight;
    using adj_container_type = typename G::adj_container_type;

    WeightedNodeGraph() : Base() {}

    WeightedNodeGraph(adj_container_type&& adjContainer)
        : Base(std::move(adjContainer)) {}

    WeightedNodeGraph(util::forward_range_of<N> auto& nodes)
        : Base(nodes) {}

    WeightedNodeGraph(util::forward_range_of<N> auto&& nodes)
        : Base(std::move(nodes)) {}
};

} // namespace MyWorkflow
