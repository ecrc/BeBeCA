/* 
 * File:   graph_t.h
 * Author: fuad
 *
 * Created on January 6, 2014, 7:55 AM
 */

#ifndef GRAPH_T_H
#define	GRAPH_T_H

#include <vector>
#include <set>
#include <utility>
#include <string>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <stack>
#include <queue>

#include "types.h"

using namespace std;

struct node_t {
    vector<node_id_t>     nbrs_vec;
};

struct graph_t;
struct component_t;

/*
 * all functions here assume the given node ids are proper ones from 0 to n-1
 * so, the caller must use 
 */
struct subgraph_t {
    vector<vector<node_id_t> >  nodes_vec;
    
    size_t      size();
    void        fill_graph(graph_t& g);
};

/*
 * Simple undirected unweighted graph data structure
 * no checks whatsoever
 * should call init_size(..) first then insert_edge(..) to populate
 * 
 * IMP: nodes have indexes from 0 to n-1
 * IMP: graph is assumed to be connected
 */
struct graph_t {
    vector<node_t>      nodes_vec;
    set<edge_t >        edge_set;
    
    void                insert_edge(node_id_t src, node_id_t dst);
    void                init_size(size_t num_nodes);
    size_t              size();
    void                read_graph(string path);
    void                i_init_internals();
};





#endif	/* GRAPH_T_H */

