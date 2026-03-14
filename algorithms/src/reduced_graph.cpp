#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

// Node types: Agent (A), Object (P)
enum class NodeType { Agent, Object };

// Graph node
struct Node {
    std::string name;
    NodeType type;
};

// Edge: from Agent to object
struct Edge {
    int from; // Agent index
    int to;   // object index
    int rank; // ranking of the object in the agent's preference list. rank=1 is f(a); rank=2 is s(a)
};

// Edge: from Agent to object
struct ReducedEdge {
    int from; // Agent index
    int to;   // object index
    bool fpost; // ranking of the object in the agent's preference list. rank=1 is f(a); rank=2 is s(a)
};

class FullGraph{
public:
    // Store nodes and edges
    std::vector<Node> nodes;
    std::vector<Edge> edges;

    // Maps for fast lookup
    std::unordered_map<std::string, int> agentIndex;
    std::unordered_map<std::string, int> objectIndex;
};



// Reduced Graph structure
class ReducedGraph {
public:
    // Store nodes and edges
    std::vector<Node> nodes;
    std::vector<ReducedEdge> edges;

    // Maps for fast lookup
    std::unordered_map<std::string, int> agentIndex;
    std::unordered_map<std::string, int> objectIndex;

    // Add agent
    void addAgent(const std::string& name) {
        agentIndex[name] = nodes.size();
        nodes.push_back({name, NodeType::Agent});
    }

    // Add object
    void addObject(const std::string& name) {
        objectIndex[name] = nodes.size();
        nodes.push_back({name, NodeType::Object});
    }

    // Add node
    void addNode(const std::string& name, const NodeType& type) {
        agentIndex[name] = nodes.size();
        nodes.push_back({name, type});
    }

    // Add reduced edges for agent: one to f(a), one to s(a)
    void addReducedEdges(const std::string& agent, const std::string& f_a, const std::string& s_a) {
        int a_idx = agentIndex[agent];
        int f_idx = objectIndex[f_a];
        int s_idx = objectIndex[s_a];
        edges.push_back({a_idx, f_idx, true});
        edges.push_back({a_idx, s_idx, false});
    }

    
    bool isAgent(int i) const {
        return nodes[i].type == NodeType::Agent;
    }

    bool isObject(int i) const {
        return nodes[i].type == NodeType::Object;
    }


    // Print graph
    void print() const {
        std::cout << "Nodes:\n";
        for (size_t i = 0; i < nodes.size(); ++i) {
            std::cout << i << ": " << nodes[i].name << " (" 
                      << (isAgent(i) ? "Agent" : "Object") << ")\n";
        }
        std::cout << "Edges:\n";
        for (const auto& e : edges) {
            std::cout << nodes[e.from].name << " -> " << nodes[e.to].name << "\n";
        }
    }
};

// Example usage
int main() {
    ReducedGraph G;

    // Add agents
    G.addAgent("Alice");
    G.addAgent("Bob");

    // Add objects
    G.addObject("Engineer");
    G.addObject("Designer");

    // For each agent, add edges to f(a) and s(a)
    G.addReducedEdges("Alice", "Engineer", "Designer");
    G.addReducedEdges("Bob", "Designer", "Engineer");

    G.print();

    return 0;
}