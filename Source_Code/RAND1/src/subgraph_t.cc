/* 
 * File:   graph_t.cc
 * Author: fuad
 * 
 * Created on Jun 23, 2014
 */
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <queue>
#include <stack>


#include "graph_t.h"
#include "bc.h"
#include "utility.h"

using namespace std;

size_t subgraph_t::size()
{
    return nodes_vec.size();
}

void subgraph_t::fill_graph(graph_t& g)
{       
    nodes_vec.clear();

    nodes_vec.resize(g.size());
    for(node_id_t i = 0; i < g.size(); ++i) {
        nodes_vec[i] = g.nodes_vec[i].nbrs_vec;
    } 
}
