#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <unordered_map>
#include <set>
#include <string>

// Node types: Agent (A), Object (P)
enum class NodeType { Agent, Object };

// Graph node
struct Node {
    std::string name;
    NodeType type;
};

// Edge: from Agent to object
struct EdgeBase {
    int from; // Agent index
    int to;   // object index
};

struct Edge : EdgeBase {
    unsigned int rank; // ranking of the object in the agent's preference list. rank=1 is f(a); rank=2 is s(a)
    Edge(int f, int t, unsigned int r) {
        from = f;
        to = t;
        rank = r;
    }
};

struct ReducedEdge : EdgeBase {
    bool fpost; // ranking of the object in the agent's preference list. rank=1 is f(a); rank=2 is s(a)
    ReducedEdge(int f, int t, bool r) {
        from = f;
        to = t;
        fpost = r;
    }
};

class BaseGraph {
protected:
    // pour vérifier qu’un rang n’est pas dupliqué pour un agent
    std::unordered_map<int,std::unordered_map<int,int>> agentRanks; // agent -> (rank -> object)

public:

    std::vector<Node> nodes;

    std::unordered_map<std::string,int> agentIndex;
    std::unordered_map<std::string,int> objectIndex;

    void addAgent(const std::string& name);

    void addObject(const std::string& name);

    void addNode(const std::string& name, NodeType type);

    bool isAgent(int i) const;

    bool isObject(int i) const;

    size_t getSizeAgentRanks(int a_idx);
};

class FullGraph: public BaseGraph{

public:
    // Store edges
    std::vector<Edge> edges;

    // Constructors
    FullGraph(
        std::initializer_list<std::string> agents,
        std::initializer_list<std::string> objects,
        std::initializer_list<std::tuple<std::string,std::string,int>> edgeList
    );

    FullGraph(
        const std::vector<std::string>& agents,
        const std::vector<std::string>& objects,
        const std::vector<std::tuple<std::string,std::string,int>>& edgeList
    );

    // Utility functions

    // Add edge
    void addEdge(const std::string& agent, const std::string& object, int rank);

    // Print graph
    void print() const;
};

class ReducedGraph : public BaseGraph{
public:
    // Store edges
    std::vector<ReducedEdge> edges;

    // Add reduced edges for agent: one to f(a), one to s(a)
    void addReducedEdges(const std::string& agent, const std::string& f_a, const std::string& s_a);

    void addReducedEdge(const std::string& agent, const std::string& object, bool fpost);

    // Print graph
    void print() const;
};

ReducedGraph reduce(const FullGraph& g);

#endif // GRAPH_HPP