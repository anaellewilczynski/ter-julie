#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include "graph.hpp"


/* --- Rules ---
1. An agent can have only one item per rank
2. An agent cannot have more edges than objects
3. The ranks must be positive
4. ReducedGraph: An agent can have at most 2 edges
5. An agent can have only one f(a)
6. An agent can have only one s(a)
7. Preferences may be partial
*/


void BaseGraph::addAgent(const std::string& name)
{
    agentIndex[name] = nodes.size();
    nodes.push_back({name, NodeType::Agent});
}

void BaseGraph::addObject(const std::string& name)
{
    objectIndex[name] = nodes.size();
    nodes.push_back({name, NodeType::Object});
}

void BaseGraph::addNode(const std::string& name, NodeType type)
{
    int idx = nodes.size();

    nodes.push_back({name,type});

    if(type == NodeType::Agent)
        agentIndex[name] = idx;
    else
        objectIndex[name] = idx;
}

bool BaseGraph::isAgent(int i) const
{
    return nodes.at(i).type == NodeType::Agent;
}

bool BaseGraph::isObject(int i) const
{
    return nodes.at(i).type == NodeType::Object;
}

size_t BaseGraph::getSizeAgentRanks(int a_idx){
    return agentRanks[a_idx].size();
}


FullGraph::FullGraph(
    std::initializer_list<std::string> agents,
    std::initializer_list<std::string> objects,
    std::initializer_list<std::tuple<std::string,std::string,int>> edgeList
){
    for(const auto& a : agents)
        addAgent(a);

    for(const auto& o : objects)
        addObject(o);

    for(const auto& e : edgeList)
        addEdge(
            std::get<0>(e),
            std::get<1>(e),
            std::get<2>(e)
        );
}

FullGraph::FullGraph(
    const std::vector<std::string>& agents,
    const std::vector<std::string>& objects,
    const std::vector<std::tuple<std::string,std::string,int>>& edgeList
) {
    for(const auto& a : agents)
        addAgent(a);

    for(const auto& o : objects)
        addObject(o);

    for(const auto& e : edgeList)
        addEdge(
            std::get<0>(e),
            std::get<1>(e),
            std::get<2>(e)
        );
}

// Add edge
void FullGraph::addEdge(const std::string& agent, const std::string& object, int rank) {
    // Rules
    int a_idx = agentIndex[agent];
    if(agentRanks[a_idx].count(rank))
        throw std::logic_error([rank, agent]() {
            std::ostringstream oss;
            oss << agent <<" already has an object with rank " << rank;
            return oss.str();
        }());

    if(agentRanks[a_idx].size() >= objectIndex.size())
        throw std::logic_error(agent +" has more edges than objects");

    int o_idx = objectIndex[object];
    edges.push_back(Edge(a_idx, o_idx, rank));
    agentRanks[a_idx][rank] = o_idx;
}

// Print graph
void FullGraph::print() const {
    std::cout << "Nodes:\n";
    for (size_t i = 0; i < nodes.size(); ++i) {
        std::cout << i << ": " << nodes.at(i).name << " (" 
                  << (isAgent(i) ? "Agent" : "Object") << ")\n";
    }
    std::cout << "Edges:\n";
    for (const auto& e : edges) {
        std::cout << nodes.at(e.from).name << " - "<< e.rank <<" -> " << nodes.at(e.to).name << "\n";
    }
}

// Add reduced edges for agent: one to f(a), one to s(a)
void ReducedGraph::addReducedEdges(const std::string& agent, const std::string& f_a, const std::string& s_a) {
    // Rules
    int a_idx = agentIndex[agent];
    if(agentRanks[a_idx].size() >= objectIndex.size())
        throw std::logic_error(agent + " has more edges than objects");
        
    size_t size =  agentRanks[a_idx].size();
    if(size > 0)
        throw std::logic_error([size, agent]() {
            std::ostringstream oss;
            oss << agent << " can have at most 2 edges in the ReducedGraph. The agent has already " << size << " egdes.";
            return oss.str();
        }());

    int f_idx = objectIndex[f_a];
    int s_idx = objectIndex[s_a];
    edges.push_back(ReducedEdge(a_idx, f_idx, true));
    edges.push_back(ReducedEdge(a_idx, s_idx, false));
    agentRanks[a_idx][0] = f_idx;
    agentRanks[a_idx][1] = s_idx;
}

void ReducedGraph::addReducedEdge(const std::string& agent, const std::string& object, bool fpost) {
    unsigned int rank = fpost ? 0 : 1;
    int a_idx = agentIndex[agent];
    // Rules
    if(agentRanks[a_idx].count(rank))
        throw std::logic_error(agent + " already has an object with the rank "+ std::to_string(rank));

    if(agentRanks[a_idx].size() >= objectIndex.size())
        throw std::logic_error(agent + " has more edges than objects");
        
    if(agentRanks[a_idx].size() >= 2)
        throw std::logic_error(agent+" can have at most 2 edges in the ReducedGraph");

    int o_idx = objectIndex[object];
    edges.push_back(ReducedEdge(a_idx, o_idx, fpost ));
    agentRanks[a_idx][rank] = o_idx;
}

// Print graph
void ReducedGraph::print() const {
    std::cout << "Nodes:\n";
    for (size_t i = 0; i < nodes.size(); ++i) {
        std::cout << i << ": " << nodes.at(i).name << " (" 
                  << (isAgent(i) ? "Agent" : "Object") << ")\n";
    }
    std::cout << "Edges:\n";
    for (const auto& e : edges) {
        std::cout << nodes.at(e.from).name << " -> " << nodes.at(e.to).name << "\n";
    }
}

ReducedGraph reduce(const FullGraph& g){
    ReducedGraph reducedGraph = ReducedGraph();
    for(size_t i = 0; i < g.nodes.size(); i++){
        reducedGraph.addNode(g.nodes.at(i).name, g.nodes.at(i).type);
    }
    for(size_t i = 0; i < g.edges.size(); i++){
        if(g.edges.at(i).rank < 3){
            bool fpost = g.edges.at(i).rank == 1 ? true : false;
            reducedGraph.addReducedEdge(g.nodes.at(g.edges.at(i).from).name, g.nodes.at(g.edges.at(i).to).name, fpost);
        }
    }
    return reducedGraph;
}
