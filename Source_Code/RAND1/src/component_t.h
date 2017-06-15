/* 
 * File:   component_t.h
 * Author: fuad
 *
 * Created on November 24, 2014, 4:08 PM
 */

#ifndef COMPONENT_T_H
#define	COMPONENT_T_H

#include "graph_t.h"

enum comp_type_t {BCC, MUC, GRAPH};

/*
 * The component (subgraph along with other needed information)
 * that bc computation blocks operate on.
 * Could be a BCC, an MUC, or just a graph.
 */
struct component_t {
    //maps articulation points to sizes of subgraphs connected to the bcc
    //through them
    
    subgraph_t      subgraph;
    comp_type_t     comp_type;
    
};

#endif	/* COMPONENT_T_H */

