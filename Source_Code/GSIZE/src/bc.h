/* 
 * File:   bc.h
 * Author: fuad
 *
 * Created on January 6, 2014, 9:27 AM
 */

#ifndef BC_H
#define	BC_H

#include "graph_t.h"

#include "iter_info_t.h"
#include "component_t.h"

using namespace std;

/*
 * One iteration of Brandes algorithm from source @s
 */
void brandes_iter(
        vector<double>& BC_vec,     // BC of vertices
        component_t&    comp,       // component could be BCC, MUC, or just a graph
        node_id_t       s,          // source of the iteration
        iter_info_t&    iter_info   //TODO to be used later?
        );

/*
 * If dd=1 will compute both old values and new values to eliminate need for
 * PartialBFS later
 * IMP: e.first is assumed to be closer to the source than e.second
 */
void BBFS(
        iter_info_t&    iter_info,  // iteration info to be computed
        component_t&    comp,       // component
        node_id_t       s          // source of the iteration
        );

/*
 *
 */
void RBFS(
        vector<double>& dBC_vec,    // delta BC of vertices
        component_t&    comp,       // component could be BCC, MUC, or just a graph
        node_id_t       s,          // source of the iteration
        iter_info_t&    iter_info,  //
        bool            add,
        bool            sub,
		int				chosen_vertex,
		double			maximum_sum
        );

/*
 * Parallel Brandes functions
 * Ziyad territory 
 */

void prepare_subgraph(
		string  graph_path,
		component_t& comp
		);

int parallel_brandes(
		component_t& comp,
        vector<double>& BC_vec,
		int chosen_vertex,
		double c_constant
        );

int brandes_block(
        vector<double>*     dBC_vec,
        component_t*        comp,
		int					chosen_vertex,
		double 				the_maximum_sum
        );

void traverse_serial_randomly(
		component_t&        comp,
		int 			number_of_verticies);

#endif	/* BC_H */
