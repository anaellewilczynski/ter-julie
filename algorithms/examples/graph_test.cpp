#include <string>
#include "graph.hpp"
#include <cassert>
#include <iostream>


// Example usage
int main() {
    // Create a valid FullGraph
    FullGraph G(
        {"Alice","Bob","Caroline"},
        {"Engineer","Designer", "Photographer"},
        {
            {"Alice","Engineer",1},
            {"Alice","Designer",2},
            {"Bob","Designer",1},
            {"Bob","Engineer",2},
            {"Caroline","Engineer",1},
            {"Caroline","Photographer",2},
            {"Caroline","Designer",3}
        }
    );

    // Print original graph
    G.print();

    // Test rule: agent cannot have duplicate rank
    try {
        G.addEdge("Alice", "Photographer", 2); // Alice already has rank 2
        assert(false && "Should throw: duplicate rank for agent");
    } catch(const std::logic_error& e) {
        std::cout << "Caught expected error: " << e.what() << "\n";
    }

    // Test rule: agent cannot have more edges than objects
    try {
        G.addEdge("Bob", "Photographer", 3); // Bob already has 2 edges, only 3 objects
        G.addEdge("Bob", "Photographer", 4); // Should throw
        assert(false && "Should throw: more edges than objects");
    } catch(const std::logic_error& e) {
        std::cout << "Caught expected error: " << e.what() << "\n";
    }

    // Reduce graph
    ReducedGraph reducedG = reduce(G);

    // Print reduced graph
    reducedG.print();

    // Test reduced graph rules
    // Each agent should have at most 2 edges
    for(const auto& node : reducedG.nodes) {
        if(node.type == NodeType::Agent) {
            int idx = reducedG.agentIndex[node.name];
            assert(reducedG.getSizeAgentRanks(idx) <= 2);
        }
    }

    // Test adding a third edge in reduced graph
    try {
        reducedG.addReducedEdge("Alice", "Photographer", false);
        assert(false && "Should throw: more than 2 edges in ReducedGraph");
    } catch(const std::logic_error& e) {
        std::cout << "Caught expected error: " << e.what() << "\n";
    }

    std::cout << "All tests passed.\n";
    return 0;
}