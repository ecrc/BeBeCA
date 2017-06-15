#include "bc.h"
#include "utility.h"

#include <thread>
#include <mpi.h>

using namespace std;

void brandes_iter(
        vector<double>& BC_vec,     // BC of vertices
        component_t&    comp,       // component could be BCC, MUC, or just a graph
        node_id_t       s,          // source of the iteration
        iter_info_t&    iter_info
        )
{
    iter_info.init_all(comp.subgraph.size());
    BBFS(iter_info, comp, s);
    RBFS(BC_vec, comp, s, iter_info, true, false);
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
        bool            sub
        )
{
    vector<vector<node_id_t> >&  P = iter_info.P;
    vector<int>&                 sigma_vec = iter_info.sigma_vec;
    vector<double>&              delta_vec = iter_info.delta_vec;
    vector<node_id_t>&           S = iter_info.S;
    
    fill_vec<double>(delta_vec, comp.subgraph.size(), 0);
    
   
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
		component_t& comp)
{
	//ZIYAD_COMMENT: These lines to read the graph
	graph_t graph;
	graph.read_graph(graph_path);

	//ZIYAD_COMMENT:Fill the component with the given graph
	comp.comp_type = GRAPH;
	comp.subgraph.fill_graph(graph);
}

void parallel_brandes(
		component_t& comp,
        vector<double>& BC_vec,
		int number_of_threads
        )
{
	//ZIYAD_COMMENT: Prepare the vector where the results will be stored and fill the vector with 0.0
    BC_vec.resize(comp.subgraph.size());
    fill(BC_vec.begin(), BC_vec.end(), 0.0);
    
	//ZIYAD_COMMENT: Get the the current node rank and how many nodes involved
	int rank, number_of_nodes ;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD,&number_of_nodes);

	//ZIYAD_COMMENT: Calculate how the vertices should be divided among the nodes
	int number_of_verticies, number_of_vericies_for_each, remaining_verticies;
	number_of_verticies = comp.subgraph.size();
	number_of_vericies_for_each = number_of_verticies/(number_of_nodes);
	remaining_verticies = number_of_verticies%(number_of_nodes);

	vector<node_id_t> verticies_reponsibile_for; //ZIYAD_COMMENT: This vector will be used to determine which vertices this process is responsible for
	int starting_vertex, added_to_starting_verticies; //ZIYAD_COMMENT: These two variables will be used to calculate vertices this node responsible for

	//ZIYAD_COMMENT: This if-else statement might add one vertex to this process in case that vertices couldn't be divided equally
	if(rank<remaining_verticies) added_to_starting_verticies = rank ;
	else added_to_starting_verticies = remaining_verticies ;

	starting_vertex = number_of_vericies_for_each*(rank) + added_to_starting_verticies ; //ZIYAD_COMMENT: The first vertex the node is responsible for

	//ZIYAD_COMMENT: This loop will prepare the vertices IDs that is under responsibility of this process
	for(int i = 0 ; i<=number_of_vericies_for_each; i++){
		if(i==number_of_vericies_for_each && rank>=remaining_verticies) break ;
		node_id_t nodeId = starting_vertex + i ;
		verticies_reponsibile_for.push_back(nodeId) ;
	}
	if(rank==0)printf("\nMaster: Graph vertices are identified") ;


	//=====================================================
	//ZIYAD_COMMENT: Start threading part
	//=====================================================

	//ZIYAD_COMMENT: These to store the created threads
	vector<std::thread> all_threads;
	all_threads.resize(number_of_threads-1); //ZIYAD_COMMENT: -1 because we will have the master thread as well already working
	all_threads.clear();

	//ZIYAD_COMMENT: These three lines to store results from threads
	vector<vector<double> > threads_results;
	threads_results.resize(number_of_threads-1); //ZIYAD_COMMENT: -1 because we will have the master thread and directly operate on
	threads_results.clear();

	//ZIYAD_COMMENT: These three lines to store vertices IDs of each thread
	vector<vector<node_id_t> > threads_vertcies;
	threads_vertcies.resize(number_of_threads);
	threads_vertcies.clear();

	//ZIYAD_COMMENT: This loop will compute vertices related to each thread, create the thread, start its task and store its result
	if(rank==0)printf("\nMaster: Start creating threads and send them graph nodes IDs") ;
	int thread_id ;
	for(thread_id=0 ; thread_id<number_of_threads ; thread_id++){
		int verticies_per_thread = verticies_reponsibile_for.size() / number_of_threads ;
		int remaining_verticies_for_threads = verticies_reponsibile_for.size() % number_of_threads ;

		int added_to_starting_verticies_threads = 0 ; //ZIYAD_COMMENT: These two variables will be used to calculate vertices this thread responsible for

		//ZIYAD_COMMENT: This if-else statement might add one vertex to this thread in case that vertices couldn't be divided equally
		if(thread_id<remaining_verticies_for_threads) added_to_starting_verticies_threads = thread_id ;
		else added_to_starting_verticies_threads = remaining_verticies_for_threads ;

		//ZIYAD_COMMENT: These couple of lines will copy only the vertices under this thread responsibilities
		vector<node_id_t>::const_iterator first = verticies_reponsibile_for.begin() + (verticies_per_thread*thread_id) + added_to_starting_verticies_threads ;
		vector<node_id_t>::const_iterator last = verticies_reponsibile_for.begin() + (verticies_per_thread*(thread_id+1)) + added_to_starting_verticies_threads ;
		vector<node_id_t> this_thread_vertices(first,last);

		if(thread_id<remaining_verticies_for_threads) this_thread_vertices.push_back(verticies_reponsibile_for[(verticies_per_thread*(thread_id+1))+added_to_starting_verticies_threads]);

		threads_vertcies.push_back(this_thread_vertices);
	}

	//ZIYAD_COMMENT: This loop is for optimization process to delete all in previous loop
	for(thread_id=0 ; thread_id<number_of_threads ; thread_id++){
		if(thread_id==number_of_threads-1) break ; //ZIYAD_COMMENT: This mean we have the remaining of work for the master thread

		//ZIYAD_COMMENT: This line will create the thread and send to it the nodes it is responsible of
		(threads_results[thread_id]).resize(number_of_verticies) ;
		fill((threads_results[thread_id]).begin(), (threads_results[thread_id]).end(), 0.0);
		all_threads[thread_id] = std::thread(brandes_block, &threads_results[thread_id], &comp, &threads_vertcies[thread_id],thread_id);
	}

	if(rank==0)printf("\nMaster: End of creating threads and send them graph nodes IDs") ;

	if(rank==0)printf("\nMaster: All threads except master started traversing from the given graph nodes") ;
	//ZIYAD_COMMENT: This is the master thread will participate in computation
	brandes_block(&BC_vec, &comp, &threads_vertcies[number_of_threads-1],number_of_threads-1) ;

	if(rank==0)printf("\nMaster: End from traversing and waiting for other threads to complete work") ;
	//ZIYAD_COMMENT: wait for the threads to finish, then accumulate the results
	for(thread_id = 0; thread_id < number_of_threads-1 ; thread_id++) {
		all_threads[thread_id].join();
	}
	if(rank==0)printf("\nMaster: All threads finish their work") ;

	//ZIYAD_COMMENT: This loop will take results from each thread and sum them to get the results for this process
	for(int j = 0 ; j < number_of_threads-1 ; j++){
		for(int i = 0 ; i < number_of_verticies ; i++ ){
			BC_vec[i] = BC_vec[i] + threads_results[j][i];
		}
	}
	if(rank==0)printf("\nMaster: Results from threads are summed") ;

	//=====================================================
	//ZIYAD_COMMENT: End of threading part
	//=====================================================


	//ZIYAD_COMMENT: This part related to the master node that is responsible to collect the results only
    if(rank==0){
    	//ZIYAD_COMMENT: This vector will be used to receive the results from each node
    	vector<double> temp ;
    	temp.resize(number_of_verticies);
    	temp.clear();

    	if(rank==0)printf("\nMaster: Waiting for results from other processes") ;
    	MPI_Status recieve_status ;
    	int receiving_rank, i ;
    	//ZIYAD_COMMENT: This loop will receive the result from each node and sum them for the final result
    	for(receiving_rank = 1 ; receiving_rank<number_of_nodes ; receiving_rank++){
        	MPI_Recv(&temp[0], number_of_verticies, MPI_DOUBLE, receiving_rank , 1, MPI_COMM_WORLD, &recieve_status ); //ZIYAD_COMMENT: Send the results to the master node
        	for(i=0; i<number_of_verticies; i++){
        		BC_vec[i] += temp[i];
        	}
    	}
    	if(rank==0)printf("\nMaster: All results from other processes are received and summed") ;

    }else{
    	//ZIYAD_COMMENT: Send the result to the master node
    	MPI_Send(&BC_vec[0], number_of_verticies, MPI_DOUBLE, 0 , 1, MPI_COMM_WORLD); //ZIYAD_COMMENT: Send the results to the master node*/
    }
}

void brandes_block(
        vector<double>*     dBC_vec,
        component_t*        comp,
        vector<node_id_t>*  source_vec,
		int threadID
        )
{

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(rank==0)printf("\nMaster: Thread ID# %d started traversing",threadID) ;

	int i ;
    //ZIYAD_COMMENT: This loop to compute between centrality from the given verticies
    for(i = 0; i < source_vec->size(); i++) {
    	iter_info_t iter_info;
        brandes_iter(*dBC_vec, *comp, (*source_vec)[i], iter_info);
    }

    //if(threadID==0)printf("\nTEST %f, %f, %f",(*dBC_vec)[0],(*dBC_vec)[5],(*dBC_vec)[10]);
}

void traverse_serial_randomly(
		component_t&        comp,
		int 			number_of_verticies){

	vector<double> notUsed ;
	notUsed.resize(comp.subgraph.size());
	iter_info_t iter_info;

	srand(time(NULL)) ;
	for(int i = 0 ; i<number_of_verticies ; i++){
		int vertex = rand() % comp.subgraph.size();
		brandes_iter(notUsed, comp, vertex, iter_info);
	}
}
