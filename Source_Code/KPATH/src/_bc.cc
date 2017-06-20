#include "bc.h"
#include "utility.h"
using namespace std;

void brandes_iter(
        vector<double>& BC_vec,     // BC of vertices
        sgraph_t&    comp,       // component could be BCC, MUC, or just a graph
        node_id_t       s,          // source of the iteration
        iter_info_t&    iter_info,
		int				k_path
        )
{
    iter_info.init_all(comp.size());
    BBFS(iter_info, comp, s, k_path);
    RBFS(BC_vec, comp, s, iter_info, true, false);
}


void BBFS(
        iter_info_t&    iter_info,  // iteration info to be computed
        sgraph_t&    comp,       // component
        node_id_t       s,          // source of the iteration
		int				k_path
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


void RBFS(
        vector<double>& dBC_vec,    // delta BC of vertices
        sgraph_t&    comp,       // component could be BCC, MUC, or just a graph
        node_id_t       s,          // source of the iteration
        iter_info_t&    iter_info,  //
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
 * Parallel Brandes functions
 * Ziyad territory 
 */
//

void prepare_subgraph(
		string  graph_path,
		sgraph_t& comp)
{
	//ZIYAD_COMMENT: These lines to read the graph
	graph_t graph;
	graph.read_graph(graph_path);

	//ZIYAD_COMMENT:Fill the component with the given graph
	comp.fill_graph(graph);
}

void parallel_brandes(
		sgraph_t& comp,
        vector<double>& BC_vec,
		int     k_path
        )
{
	//ZIYAD_COMMENT: Prepare the vector where the results will be stored and fill the vector with 0.0
    	BC_vec.resize(comp.size());
    	fill(BC_vec.begin(), BC_vec.end(), 0.0);

	printf("\nMaster: Start traversing") ;
	//ZIYAD_COMMENT: This is the master thread will participate in computation
	brandes_block(&BC_vec, &comp, k_path) ;

}

void brandes_block(
        vector<double>*     dBC_vec,
        sgraph_t*        comp,
		int					k_path
        )
{
	int i ;
    //ZIYAD_COMMENT: This loop to compute between centrality from the given verticies
    for(i = 0; i < comp->size(); i++) {
    	iter_info_t iter_info;
        brandes_iter(*dBC_vec, *comp, i, iter_info, k_path);
    }
}

