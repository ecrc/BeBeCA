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
#include "graph_t.h"

using namespace std;

//---------------------------------		
/*
 * Parallel Brandes functions
 */
//---------------------------------		
void prepare_sgraph(
		string  graph_path,
		sgraph_t& comp
		);

void brandes_iter(
        vector<double>& BC_vec,  
        sgraph_t&    comp,     
        node_id_t       s,        
        iter_info_t&    iter_info 
        );
		
void BBFS(
        iter_info_t&    iter_info, 
        sgraph_t&    comp,       
        node_id_t       s          
        );
		
void RBFS(
        vector<double>& dBC_vec,    
        sgraph_t&    comp,       
        node_id_t       s,          
        iter_info_t&    iter_info,  
        bool            add,
        bool            sub
        );

void parallel_brandes(
		sgraph_t&        comp,
        vector<double>& BC_vec,
		int numberOfThreads
        );

void brandes_block(
        vector<double>*     dBC_vec,
        sgraph_t*        comp,
        vector<node_id_t>*  source_vec,
		int threadID
        );

void traverse_serial_randomly(
		sgraph_t&        comp,
		int 			number_of_verticies);
		
//---------------------------------		
/*
 * The following methods for RAND1
 */			
//---------------------------------		
void RAND1(
		sgraph_t&        comp,
        vector<double>& BC_vec,
		int sampleSize
        );
		
void brandes_block_RAND1(
        vector<double>*     dBC_vec,
        sgraph_t*        comp,
        vector<node_id_t>*  source_vec,
		int cons
        );

void brandes_iter_RAND1(
        vector<double>& BC_vec,     
        sgraph_t&    comp,       
        node_id_t       s,          
        iter_info_t&    iter_info   
        );
		
void BBFS_RAND1(
        iter_info_t&    iter_info,  
        sgraph_t&    comp,       
        node_id_t       s          
        );

void RBFS_RAND1(
        vector<double>& dBC_vec,   
        sgraph_t&    comp,      
        node_id_t       s,          
        iter_info_t&    iter_info,  
        bool            add,
        bool            sub
        );		
		
		
//---------------------------------		
/*
 * The following methods for GSIZE
 */			
//---------------------------------		
int GSIZE(
		sgraph_t& comp,
        vector<double>& BC_vec,
		int chosen_vertex,
		double c_constant
        );

int brandes_block_GSIZE(
        vector<double>*     dBC_vec,
        sgraph_t*        	comp,
		int					chosen_vertex,
		double 				the_maximum_sum
        );
		
void brandes_iter_GSIZE(
        vector<double>& BC_vec,  
        sgraph_t&    comp,      
        node_id_t       s,         
        iter_info_t&    iter_info,
		int				chosen_vertex,
		double			maximum_sum
        );
		
void BBFS_GSIZE(
        iter_info_t&    iter_info,
        sgraph_t&    comp,   
        node_id_t       s  
        );
		
void RBFS_GSIZE(
        vector<double>& dBC_vec,    
        sgraph_t&    comp,   
        node_id_t       s,   
        iter_info_t&    iter_info, 
        bool            add,
        bool            sub,
		int				chosen_vertex,
		double			maximum_sum
        );
		

//---------------------------------				
/*
These methods for the KPATH and EGO algorithms
*/	
//---------------------------------				
void limited_traverse(
		sgraph_t& comp,
        vector<double>& BC_vec,
		int     k_path
        );
		
void brandes_block_limited(
        vector<double>*     dBC_vec,
        sgraph_t*        comp,
		int					k_path
        );

void brandes_iter_limited(
        vector<double>& BC_vec,     
        sgraph_t&    comp,       
        node_id_t       s,         
        iter_info_t&    iter_info,
		int				k_path
        );


void BBFS_limited(
        iter_info_t&    iter_info, 
        sgraph_t&    comp,      
        node_id_t       s,          
		int				k_path
        );


void RBFS_limited(
        vector<double>& dBC_vec,    
        sgraph_t&    comp,      
        node_id_t       s,        
        iter_info_t&    iter_info,  
        bool            add,
        bool            sub
        );

#endif	/* BC_H */
