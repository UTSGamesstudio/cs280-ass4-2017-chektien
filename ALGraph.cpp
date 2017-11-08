#include "ALGraph.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>

#define DEBUG_PQ

using std::cout;
using std::endl;

ALGraph::ALGraph(unsigned size): size_{size}{
    for (unsigned i=0; i<size_; ++i)
        alist_.push_back(std::vector<AdjInfo>());
}

ALGraph::~ALGraph(void){
}

void ALGraph::AddDEdge(unsigned source, unsigned destination, unsigned weight){
    auto i = source-1;
    alist_[i].push_back(AdjInfo{destination, weight});
    std::sort(alist_[i].begin(), alist_[i].end());
}

void ALGraph::AddUEdge(unsigned node1, unsigned node2, unsigned weight){
    AddDEdge(node1, node2, weight);
    AddDEdge(node2, node1, weight);
}

std::vector<DijkstraInfo> ALGraph::Dijkstra(unsigned start_node) const{

    // node info to store working data related to Dijkstra's
    struct NodeInfo {
        unsigned id;
        unsigned cost; // cost from start node
        unsigned prev; // id of prev node that results in above cost

        // reverse the < to establish min heap
        bool operator<(const NodeInfo& rhs) const {
            return cost > rhs.cost;       
        }

        bool operator==(const unsigned& val) const {
            return id == val;
        }
    };

    // init all node info and push into a heap
    std::vector<NodeInfo> nis;
    for (unsigned i=0; i<alist_.size(); ++i) {
        auto id = i+1;
        unsigned cost = 0;
        if (id != start_node) 
            cost = std::numeric_limits<unsigned>::max();
        nis.push_back(NodeInfo{id, cost, id});
    }
    std::make_heap(nis.begin(), nis.end());
       
    // to denote completed nodes
    bool is_completed[size_];
    for (auto& it: is_completed)
        it = false;
    
#ifdef DEBUG_PQ
    cout << "Dijkstra: PQ contents:" << endl;
    for (auto it: nis)
        cout << it.id << ": cost=" << it.cost << ", prev=" << it.prev << endl;
    //std::pop_heap(nis.begin(), nis.end());
    //auto min = nis.back(); 
    //cout << "MIN node " <<  min.id << ": cost=" << min.cost << ", prev=" << min.prev << endl;
#endif

    // run Dijkstra's
    std::vector<NodeInfo> nis_completed(size_);
    while (!nis.empty()) {
        // pop current node info from heap
        std::pop_heap(nis.begin(), nis.end());
        auto ni = nis.back();
        nis.pop_back();
        nis_completed[ni.id-1] = ni;
        is_completed[ni.id-1] = true;

#ifdef DEBUG_PQ
        cout << "\nDijkstra: working on node: " << ni.id << endl;
        cout << "is_completed=";
        for (unsigned i=0; i<size_; ++i)
            cout << is_completed[i] << " ";
        cout << endl;
#endif

        // update all outgoing nodes
        auto i = ni.id - 1;
        for (auto ai: alist_[i]) {

            auto ni_adj = std::find(nis.begin(), nis.end(), ai.id);
            auto new_cost = ni.cost + ai.weight;


            if (!is_completed[ai.id-1] && new_cost < ni_adj->cost) {

#ifdef DEBUG_PQ
                cout << "Dijkstra: updating adj node: " << ai.id << ", old_cost=" << ni_adj->cost << " new_cost=" << new_cost << endl;
#endif

                ni_adj->cost = new_cost;
                ni_adj->prev = ni.id;

                // decrease_key in heap
                std::make_heap(nis.begin(), nis.end());
            }
        }
       
#ifdef DEBUG_PQ
        cout << "Dijkstra: PQ contents after done with node :" << ni.id << endl;
        for (auto it: nis)
            cout << it.id << ": cost=" << it.cost << ", prev=" << it.prev << endl;
#endif
    
    }

#ifdef DEBUG_PQ
    cout << "Dijkstra: completed nodes:" << endl;
    for (auto it: nis_completed)
        cout << it.id << ": cost=" << it.cost << ", prev=" << it.prev << endl;
#endif

    // make paths by backtracking
    std::vector<DijkstraInfo> path_nodes(size_);
    for (auto ni: nis_completed) {
        auto i = ni.id-1;
        auto ni_prev = ni;
        auto id_prev = ni_prev.id;
        std::vector<unsigned> path;
        path.push_back(id_prev);

#ifdef DEBUG_PQ
        cout << "Dijkstra: backtracking path for " << ni.id << " prev=" << id_prev << endl;
#endif

        while (id_prev != start_node) {
            ni_prev = nis_completed[id_prev-1]; 
            id_prev = ni_prev.prev; 
            path.push_back(id_prev);
        }
        std::reverse(path.begin(), path.end());
        path_nodes[i] = DijkstraInfo{ni.cost, path};
    }

    return path_nodes;
}

ALIST ALGraph::GetAList(void) const{
    // copy into driver communicable structures
    ALIST alist;
    for (auto ilist: alist_) {
        alist.push_back(std::vector<AdjacencyInfo>());
        for (auto item: ilist)
            alist.back().push_back(AdjacencyInfo{item.id, item.weight});
    }
    return alist;
}
