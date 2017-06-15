/* 
 * File:   graph_t.cc
 * Author: fuad
 * 
 * Created on January 6, 2014, 7:55 AM
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

    
void graph_t::insert_edge(node_id_t src, node_id_t dst)
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

void graph_t::init_size(size_t num_nodes)
{
    nodes_vec.resize(num_nodes);
}

size_t graph_t::size()
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
void graph_t::read_graph(string path)
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
        //TODO could this cause issues?
        if(src < N && dst < N) {
            insert_edge(src, dst);
        }
    }
    fin.close();
}



