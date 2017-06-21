#include "bc.h"
#include "utility.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>

using namespace std;

void prepare_sgraph(
		string  graph_path,
		sgraph_t& comp)
{
	//ZIYAD_COMMENT: These lines to read the graph
	graph_t graph;
	graph.read_graph(graph_path);

	//ZIYAD_COMMENT:Fill the component with the given graph
	comp.fill_graph(graph);
}


/*
These methods for the RAND1 algorithm
*/
void RAND1(
		sgraph_t& comp,
        vector<double>& BC_vec,
		int sample_size
        )
{
	//ZIYAD_COMMENT: Prepare the vector where the results will be stored and fill the vector with 0.0
    BC_vec.resize(comp.size());
    fill(BC_vec.begin(), BC_vec.end(), 0.0);

	/* initialize random seed: */
	srand (time(NULL));

	printf("\nMaster: Start sampling vertices") ;
	vector<node_id_t> chosenVertices ;
	for(int i = 0 ; i < sample_size ; ){
		/* generate secret number between 0 and graph size: */
		int vertex = rand() % comp.size() ;
		if(std::find(chosenVertices.begin(), chosenVertices.end(), vertex) != chosenVertices.end()) {
		    continue ;
		} else {
			chosenVertices.push_back(vertex);
			i++ ;
		}
	}

	printf("\nMaster: Start estimating betweenness from samppled vertices") ;
	brandes_block_RAND1(&BC_vec, &comp, &chosenVertices, 0) ;

	for(int i = 0 ; i < BC_vec.size() ; i++){
		BC_vec[i] = comp.size()*(BC_vec[i]/sample_size) ;
	}
}

void brandes_block_RAND1(
        vector<double>*     dBC_vec,
        sgraph_t*        comp,
        vector<node_id_t>*  source_vec,
		int cons
        )
{
	int i ;
	//ZIYAD_COMMENT: This loop to compute between centrality from the given verticies
	for(i = 0; i < source_vec->size(); i++) {
		iter_info_t iter_info;
		brandes_iter_RAND1(*dBC_vec, *comp, (*source_vec)[i], iter_info);
	}
}

void brandes_iter_RAND1(
        vector<double>& BC_vec,     // BC of vertices
        sgraph_t&    comp,          
        node_id_t       s,          // source of the iteration
        iter_info_t&    iter_info
        )
{
    iter_info.init_all(comp.size());
    BBFS_RAND1(iter_info, comp, s);
    RBFS_RAND1(BC_vec, comp, s, iter_info, true, false);
}

void BBFS_RAND1(
        iter_info_t&    iter_info,  // iteration info to be computed
        sgraph_t&    comp,   	    
        node_id_t       s           
        )
{
    sgraph_t& g = comp;
    
    vector<vector<node_id_t> >&  P = iter_info.P;
    vector<int>&                 sigma_vec = iter_info.sigma_vec;
    vector<int>&                 dist_vec = iter_info.dist_vec;
    vector<node_id_t>&           S = iter_info.S;
    
    queue<node_id_t>             Q;
    
    // Assumes iter_info is initialized
    sigma_vec[s] = 1;
    dist_vec[s] = 0;
    Q.push(s);
    while(!Q.empty()) {
        node_id_t v_i = Q.front(); Q.pop();
        S.push_back(v_i);
        for(int i = 0; i < g.nodes_vec[v_i].size(); ++i) {
            node_id_t v_n = g.nodes_vec[v_i][i];
            if(dist_vec[v_n] < 0) {
                Q.push(v_n);
                dist_vec[v_n] = dist_vec[v_i] + 1;
            }
            if(dist_vec[v_n] == dist_vec[v_i] + 1) {
                sigma_vec[v_n] = sigma_vec[v_n] + sigma_vec[v_i];
                P[v_n].push_back(v_i);
            }
        }
    }
}


void RBFS_RAND1(
        vector<double>& dBC_vec,    
        sgraph_t&    comp,       
        node_id_t       s,          
        iter_info_t&    iter_info,  
        bool            add,
        bool            sub
        )
{
    vector<vector<node_id_t> >&  P = iter_info.P;
    vector<int>&                 sigma_vec = iter_info.sigma_vec;
    vector<double>&              delta_vec = iter_info.delta_vec;
    vector<node_id_t>&           S = iter_info.S;
    
    fill_vec<double>(delta_vec, comp.size(), 0);
    
   
	for(int i = S.size()-1; i >= 0; --i) {
		node_id_t w = S[i];
		for (int idx = 0; idx < P[w].size(); ++idx) {
			node_id_t v = P[w][idx];
			delta_vec[v] += (((double) sigma_vec[v] / sigma_vec[w]) * (1 + delta_vec[w]));
		}
		if (w != s) {
			if(add) dBC_vec[w] += delta_vec[w]/2.0;
			if(sub) dBC_vec[w] -= delta_vec[w]/2.0;
		}
	}
}


/*
These methods for the GSIZE algorithm
*/
int GSIZE(
		sgraph_t& comp,
        vector<double>& BC_vec,
		int chosen_vertex,
		double c_constant
        )
{
    BC_vec.resize(comp.size());
    fill(BC_vec.begin(), BC_vec.end(), 0.0);

	return brandes_block_GSIZE(&BC_vec, &comp, chosen_vertex, (c_constant*comp.size())) ;
}

int brandes_block_GSIZE(
        vector<double>*     dBC_vec,
        sgraph_t*           comp,
		int					chosen_vertex,
		double 				the_maximum_sum
        )
{
	int random_vertex ;
	srand (time(NULL));

	vector<int> already_chosen ;
	iter_info_t iter_info;
	iter_info.sum_of_dependancies = 0 ;

	for(int i = 0 ; i < comp->size() ; ){
		if(iter_info.sum_of_dependancies>the_maximum_sum) break ;
		if(already_chosen.size()>=comp->size()) break ;
		random_vertex = rand() % comp->size() ;
		if(std::find(already_chosen.begin(), already_chosen.end(), random_vertex) != already_chosen.end()) {
			continue ;
		} else {
			already_chosen.push_back(random_vertex) ;
			brandes_iter_GSIZE(*dBC_vec, *comp, random_vertex, iter_info, chosen_vertex, the_maximum_sum);
			i++;
		}
	}


	for(int i = 0 ; i < dBC_vec->size() ; i++){
		(*dBC_vec)[i] = (comp->size()*(*dBC_vec)[i])/already_chosen.size() ;
	}

	printf("\nMaster: %d of samples have been used",already_chosen.size()) ;
	return already_chosen.size() ;
}

void brandes_iter_GSIZE(
        vector<double>& BC_vec,     
        sgraph_t&    comp,       
        node_id_t       s,          
        iter_info_t&    iter_info,
		int				chosen_vertex,
		double			maximum_sum
        )
{
    iter_info.init_all(comp.size());
    BBFS_GSIZE(iter_info, comp, s);
    RBFS_GSIZE(BC_vec, comp, s, iter_info, true, false, chosen_vertex, maximum_sum);
}

void BBFS_GSIZE(
        iter_info_t&    iter_info,  
        sgraph_t&    comp,       
        node_id_t       s          
        )
{
    sgraph_t& g = comp;
    
    vector<vector<node_id_t> >&  P = iter_info.P;
    vector<int>&                 sigma_vec = iter_info.sigma_vec;
    vector<int>&                 dist_vec = iter_info.dist_vec;
    vector<node_id_t>&           S = iter_info.S;    
    queue<node_id_t>             Q;
    
    sigma_vec[s] = 1;
    dist_vec[s] = 0;
    Q.push(s);
    while(!Q.empty()) {
        node_id_t v_i = Q.front(); Q.pop();
        S.push_back(v_i);
        for(int i = 0; i < g.nodes_vec[v_i].size(); ++i) {
            node_id_t v_n = g.nodes_vec[v_i][i];
            if(dist_vec[v_n] < 0) {
                Q.push(v_n);
                dist_vec[v_n] = dist_vec[v_i] + 1;
            }
            if(dist_vec[v_n] == dist_vec[v_i] + 1) {
                sigma_vec[v_n] = sigma_vec[v_n] + sigma_vec[v_i];
                P[v_n].push_back(v_i);
            }
        }
    }
}


void RBFS_GSIZE(
        vector<double>& dBC_vec,    
        sgraph_t&    comp,       
        node_id_t       s,          
        iter_info_t&    iter_info,  
        bool            add,
        bool            sub,
		int				chosen_vertex,
		double			maximum_sum
        )
{
    vector<vector<node_id_t> >&  P = iter_info.P;
    vector<int>&                 sigma_vec = iter_info.sigma_vec;
    vector<double>&              delta_vec = iter_info.delta_vec;
    vector<node_id_t>&           S = iter_info.S;
    
    fill_vec<double>(delta_vec, comp.size(), 0);
    
    double 						 sum_of_dependancies = iter_info.sum_of_dependancies ;

	for(int i = S.size()-1; i >= 0; --i) {
		node_id_t w = S[i];
		for (int idx = 0; idx < P[w].size(); ++idx) {
			node_id_t v = P[w][idx];
			delta_vec[v] += (((double) sigma_vec[v] / sigma_vec[w]) * (1 + delta_vec[w]));
		}
		if (w != s) {

			if(w == chosen_vertex) sum_of_dependancies += delta_vec[w]/2.0 ;
			if(sum_of_dependancies>maximum_sum) break ;
			
			if(add) dBC_vec[w] += delta_vec[w]/2.0;
			if(sub) dBC_vec[w] -= delta_vec[w]/2.0;
		}
		if(sum_of_dependancies>maximum_sum) break ;
	}

	iter_info.sum_of_dependancies = sum_of_dependancies ;
}


/*
These methods for the KPATH and EGO algorithms
*/
void limited_traverse(
		sgraph_t& comp,
        vector<double>& BC_vec,
		int     k_path
        )
{
    BC_vec.resize(comp.size());
    fill(BC_vec.begin(), BC_vec.end(), 0.0);

	printf("\nMaster: Start traversing") ;
	brandes_block_limited(&BC_vec, &comp, k_path) ;
}

void brandes_block_limited(
        vector<double>*     dBC_vec,
        sgraph_t*        comp,
		int					k_path
        )
{
	int i ;
    for(i = 0; i < comp->size(); i++) {
    	iter_info_t iter_info;
        brandes_iter_limited(*dBC_vec, *comp, i, iter_info, k_path);
    }
}

void brandes_iter_limited(
        vector<double>& BC_vec,     
        sgraph_t&    comp,       
        node_id_t       s,         
        iter_info_t&    iter_info,
		int				k_path
        )
{
    iter_info.init_all(comp.size());
    BBFS_limited(iter_info, comp, s, k_path);
    RBFS_limited(BC_vec, comp, s, iter_info, true, false);
}


void BBFS_limited(
        iter_info_t&    iter_info,  
        sgraph_t&    comp,       
        node_id_t       s,         
		int				k_path
        )
{
    sgraph_t& g = comp;
    
    vector<vector<node_id_t> >&  P = iter_info.P;
    vector<int>&                 sigma_vec = iter_info.sigma_vec;
    vector<int>&                 dist_vec = iter_info.dist_vec;
    vector<node_id_t>&           S = iter_info.S;
    
    queue<node_id_t>             Q;
    
    sigma_vec[s] = 1;
    dist_vec[s] = 0;
    Q.push(s);
    while(!Q.empty()) {
        node_id_t v_i = Q.front(); Q.pop();
        S.push_back(v_i);
        for(int i = 0; i < g.nodes_vec[v_i].size(); ++i) {
            node_id_t v_n = g.nodes_vec[v_i][i];
            if(dist_vec[v_n] < 0) {
                dist_vec[v_n] = dist_vec[v_i] + 1;
                if(dist_vec[v_n]<=k_path)Q.push(v_n);		//Here is the restriction for k-betweenness
            }
            if(dist_vec[v_n] == dist_vec[v_i] + 1) {
                sigma_vec[v_n] = sigma_vec[v_n] + sigma_vec[v_i];
                P[v_n].push_back(v_i);
            }
        }
    }
}


void RBFS_limited(
        vector<double>& dBC_vec,    
        sgraph_t&    comp,       
        node_id_t       s,          
        iter_info_t&    iter_info,  
        bool            add,
        bool            sub
        )
{
    vector<vector<node_id_t> >&  P = iter_info.P;
    vector<int>&                 sigma_vec = iter_info.sigma_vec;
    vector<double>&              delta_vec = iter_info.delta_vec;
    vector<node_id_t>&           S = iter_info.S;
    
    fill_vec<double>(delta_vec, comp.size(), 0);
    
   
	for(int i = S.size()-1; i >= 0; --i) {
		node_id_t w = S[i];
		for (int idx = 0; idx < P[w].size(); ++idx) {
			node_id_t v = P[w][idx];
			delta_vec[v] += (((double) sigma_vec[v] / sigma_vec[w]) * (1 + delta_vec[w]));
		}
		if (w != s) {
			if(add) dBC_vec[w] += delta_vec[w]/2.0;
			if(sub) dBC_vec[w] -= delta_vec[w]/2.0;
		}
	}
}