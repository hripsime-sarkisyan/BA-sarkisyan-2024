#pragma once
#include "fishnet/SimpleGraph.hpp"
#include <fishnet/GraphModel.hpp>
#include <fishnet/DirectedAcyclicGraph.hpp>
#include <fishnet/HashConcepts.hpp>
#include <fishnet/NetworkConcepts.hpp>
#include <fishnet/AdjacencyContainer.hpp>
#include <fishnet/Edge.hpp>
#include "WeightedNode.hpp"

using namespace fishnet;

namespace MyWorkflow {

class GraphFactory{
public:
template<
    typename N, 
    typename W,
    util::HashFunction<MyWorkflow::WeightedNode<N,W>> Hash = typename MyWorkflow::WeightedNode<N,W>::Hash,
    fishnet::graph::NodeBiPredicate<MyWorkflow::WeightedNode<N,W>> Equal = std::equal_to<MyWorkflow::WeightedNode<N,W>>
>
static auto WeightedNodeGraph() {
    using WeightedN = MyWorkflow::WeightedNode<N,W>;
    return graph::__impl::SimpleGraph<
        UndirectedEdge<WeightedN,Hash,Equal>,
        AdjacencyMap<WeightedN,Hash,Equal>
    >();
}

template<typename N, typename W, typename AdjacencyContainer>
static auto WeightedNodeGraph(AdjacencyContainer && adjContainer) {
    using WeightedN = MyWorkflow::WeightedNode<N,W>;
    using AdjacencyContainer_t = std::remove_cvref_t<decltype(adjContainer)>;
    using Hash = typename AdjacencyContainer_t::hash_function;
    using Equal = typename AdjacencyContainer_t::equality_predicate;

    return graph::__impl::SimpleGraph<
        UndirectedEdge<WeightedN,Hash,Equal>,
        AdjacencyContainer_t
    >(std::forward<AdjacencyContainer>(adjContainer));
}

};
} 
