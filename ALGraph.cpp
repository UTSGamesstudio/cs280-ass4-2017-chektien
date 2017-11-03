#include "ALGraph.h"

ALGraph::ALGraph(unsigned size){
    for (unsigned i=0; i<size; ++i)
        alist_.push_back(std::vector<AdjacencyInfo>());
}

ALGraph::~ALGraph(void){
}

void ALGraph::AddDEdge(unsigned source, unsigned destination, unsigned weight){
}

void ALGraph::AddUEdge(unsigned node1, unsigned node2, unsigned weight){
}

std::vector<DijkstraInfo> ALGraph::Dijkstra(unsigned start_node) const{
    std::vector<DijkstraInfo> path_nodes;
    return path_nodes;
}

ALIST ALGraph::GetAList(void) const{
    ALIST alist;
    return alist;
}
