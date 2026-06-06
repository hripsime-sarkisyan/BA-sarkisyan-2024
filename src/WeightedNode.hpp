#pragma once
#include <functional>

namespace MyWorkflow {

template<typename BaseNode, typename WeightT>
class WeightedNode {
public:
    using base_node_type = BaseNode;
    using weight_type = WeightT;

    WeightedNode() = default;
    WeightedNode(const BaseNode& node, const WeightT& w) 
        : node(node), weight(w) {}
    WeightedNode(BaseNode&& node, WeightT&& w) 
        : node(std::move(node)), weight(std::move(w)) {}

    const BaseNode& getNode() const noexcept { return node; }
    const WeightT& getWeight() const noexcept { return weight; }

    bool operator==(const WeightedNode& other) const noexcept {
        return node == other.node && weight == other.weight;
    }

    struct Hash {
        std::size_t operator()(const WeightedNode& wn) const {
            std::size_t h1 = std::hash<BaseNode>{}(wn.node);
            std::size_t h2 = std::hash<WeightT>{}(wn.weight);
            return h1 ^ (h2 << 1);
        }
    };

private:
    BaseNode node;
    WeightT weight{};
};

} // namespace MyWorkflow
