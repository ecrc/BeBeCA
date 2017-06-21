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
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include "types.h"
#include "utility.h"

using namespace std;

struct node_t {
    vector<node_id_t>     nbrs_vec;
};

struct graph_t {
    vector<node_t>      nodes_vec;
    set<edge_t >        edge_set;
    
    void                insert_edge(node_id_t src, node_id_t dst);
    void                init_size(size_t num_nodes);
    size_t              size();
    void                read_graph(string path);
    void                i_init_internals();
};


inline void graph_t::insert_edge(node_id_t src, node_id_t dst)
{
    pair<node_id_t, node_id_t> edge = make_pair(src, dst);
    if(edge_set.find(edge) != edge_set.end())
        return;
    edge = make_pair(dst, src);
    if(edge_set.find(edge) != edge_set.end())
        return;
    edge_set.insert(edge);
    nodes_vec[src].nbrs_vec.push_back(dst);
    nodes_vec[dst].nbrs_vec.push_back(src);
}

inline void graph_t::init_size(size_t num_nodes)
{
    nodes_vec.resize(num_nodes);
}

inline size_t graph_t::size()
{
    return nodes_vec.size();
}

/*
 * #vertices    #edges
 * src1         dst1
 * .
 * .
 * src#edges    dst#edges
 */
inline void graph_t::read_graph(string path)
{
    ifstream fin;
    fin.open(path.c_str(), ios::in);
    if(!fin.good()) {
        printf("Can't open the file [%s]\n", path.c_str());
        exit(1);
    }
    size_t N, M;
    fin >> N >> M;
    init_size(N);
    for(int i = 0; i < M; ++i) {
        node_id_t src, dst;
        fin >> src >> dst;
        if(src < N && dst < N) {
            insert_edge(src, dst);
        }
    }
    fin.close();
}


struct sgraph_t {
    vector<vector<node_id_t> >  nodes_vec;
    
    size_t      size();
    void        fill_graph(graph_t& g);
};

inline size_t sgraph_t::size()
{
    return nodes_vec.size();
}

inline void sgraph_t::fill_graph(graph_t& g)
{       
    nodes_vec.clear();

    nodes_vec.resize(g.size());
    for(node_id_t i = 0; i < g.size(); ++i) {
        nodes_vec[i] = g.nodes_vec[i].nbrs_vec;
    } 
}

#endif	/* GRAPH_T_H */

