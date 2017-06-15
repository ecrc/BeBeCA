#include "bc.h"
#include "utility.h"
#include <time.h>    
#include <algorithm>

using namespace std;

void brandes_iter(
        vector<double>& BC_vec,     // BC of vertices
        component_t&    comp,       // component could be BCC, MUC, or just a graph
        node_id_t       s,          // source of the iteration
        iter_info_t&    iter_info,
		int				chosen_vertex,
		double			maximum_sum
        )
{
    iter_info.init_all(comp.subgraph.size());
    BBFS(iter_info, comp, s);
    RBFS(BC_vec, comp, s, iter_info, true, false, chosen_vertex, maximum_sum);
}


void BBFS(
        iter_info_t&    iter_info,  // iteration info to be computed
        component_t&    comp,       // component
        node_id_t       s          // source of the iteration
        )
{
    subgraph_t& g = comp.subgraph;
    
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


void RBFS(
        vector<double>& dBC_vec,    // delta BC of vertices
        component_t&    comp,       // component could be BCC, MUC, or just a graph
        node_id_t       s,          // source of the iteration
        iter_info_t&    iter_info,  //
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
    
    fill_vec<double>(delta_vec, comp.subgraph.size(), 0);
    
    double 						 sum_of_dependancies = iter_info.sum_of_dependancies ;

	for(int i = S.size()-1; i >= 0; --i) {
		node_id_t w = S[i];
		for (int idx = 0; idx < P[w].size(); ++idx) {
			node_id_t v = P[w][idx];
			delta_vec[v] += (((double) sigma_vec[v] / sigma_vec[w]) * (1 + delta_vec[w]));

			//if(w == s && v == chosen_vertex) sum_of_dependancies += delta_vec[v] ;
			//if(sum_of_dependancies>maximum_sum) break ;

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
 * Parallel Brandes functions
 * Ziyad territory 
 */
//

void prepare_subgraph(
		string  graph_path,
		component_t& comp)
{
	//ZIYAD_COMMENT: These lines to read the graph
	graph_t graph;
	graph.read_graph(graph_path);

	//ZIYAD_COMMENT:Fill the component with the given graph
	comp.comp_type = GRAPH;
	comp.subgraph.fill_graph(graph);
}

int parallel_brandes(
		component_t& comp,
        vector<double>& BC_vec,
		int chosen_vertex,
		double c_constant
        )
{
	//ZIYAD_COMMENT: Prepare the vector where the results will be stored and fill the vector with 0.0
    	BC_vec.resize(comp.subgraph.size());
    	fill(BC_vec.begin(), BC_vec.end(), 0.0);

	//ZIYAD_COMMENT: This is the master thread will participate in computation
	return brandes_block(&BC_vec, &comp, chosen_vertex, (c_constant*comp.subgraph.size())) ;
}

int  brandes_block(
        vector<double>*     dBC_vec,
        component_t*        comp,
		int					chosen_vertex,
		double 				the_maximum_sum
        )
{
	int random_vertex ;
	/* initialize random seed: */
	srand (time(NULL));

	vector<int> already_chosen ;
	iter_info_t iter_info;
	iter_info.sum_of_dependancies = 0 ;

	for(int i = 0 ; i < comp->subgraph.size() ; ){
		if(iter_info.sum_of_dependancies>the_maximum_sum) break ;
		if(already_chosen.size()>=comp->subgraph.size()) break ;
		random_vertex = rand() % comp->subgraph.size() ;
		if(std::find(already_chosen.begin(), already_chosen.end(), random_vertex) != already_chosen.end()) {
			continue ;
		} else {
			already_chosen.push_back(random_vertex) ;
			brandes_iter(*dBC_vec, *comp, random_vertex, iter_info, chosen_vertex, the_maximum_sum);
			i++;
		}
	}


	for(int i = 0 ; i < dBC_vec->size() ; i++){
		(*dBC_vec)[i] = (comp->subgraph.size()*(*dBC_vec)[i])/already_chosen.size() ;
	}


	printf("\nMaster: %d of samples have been used",already_chosen.size()) ;

	return already_chosen.size() ;
}

