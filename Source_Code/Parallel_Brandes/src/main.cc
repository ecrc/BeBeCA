#include <iostream>
#include <stdio.h>
#include <fstream>
#include <list>
#include <algorithm>
#include <cmath>


#include "graph_t.h"
#include "bc.h"
#include "utility.h"

//start ZIYAD_CHANGE
#include "mpi.h"
#include <ctime>
//end ZIYAD_CHANGE
using namespace std;


/*
 *
 */
void run_parallel_brandes(
        string  graph_path,
        char  output_path[],
        int     num_threads
        )
{
	//ZIYAD_COMMENT: Note that all part related to the serial computation is commented and it can be used for verifying the results only

	component_t comp;
	prepare_subgraph(graph_path,comp) ;

	//ZIYAD_COMMENT: These two vector to store the results
	//vector<double> serialBetweenCentralityResult ;
	vector<double> parallel_between_centrality_result ;

	int rank, number_of_nodes;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD,&number_of_nodes);

	//ZIYAD_COMMENT: to compute between centality in serial of parallel fashion depend on how many nodes involved
	if(num_threads<0){
		if(rank==0){
			timer tm ;
			int number_of_vercies = (num_threads*-1) ;
			printf("Traverse From %d Random Vertices for Graph of Size %d \n",number_of_vercies, comp.subgraph.size()) ;
			tm.start();
			traverse_serial_randomly(comp,number_of_vercies);
			tm.stop();
			printf("Traversing From %d Vertices Took on Average %.4f Seconds\n",number_of_vercies, tm.interval()/number_of_vercies) ;
		}
	}
	else if(num_threads==0) {
		if(rank==0){
			printf("Getting Correct Serial Results\n") ;
			//parallel_between_centrality_result = brandes_betweenness_centrality(graph);
		}
	}
	else
	{
		if(rank==0){
			printf("===================================================================================") ;
			printf("\nComputation is Running with %d Nodes and %d Threads\n",number_of_nodes,num_threads) ;
		}
		timer tm ;
		tm.start();
		parallel_brandes(comp, parallel_between_centrality_result,num_threads);
		tm.stop();

		std::string graph_name = extract_graph_name(graph_path) ;
		printf("\n@STAT	Alg#	Brandes	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), comp.subgraph.size()) ;	
	}

	//ZIYAD_COMMENT:Store results of both serial and parallel implementation for comparisons
	if(rank==0){
		FILE *parallelBrandesOutput ;
		parallelBrandesOutput = fopen(output_path,"w");
		for(int i = 0 ; i<parallel_between_centrality_result.size() ; i++){
			fprintf(parallelBrandesOutput,"%d:	     %0.12f\n",i,parallel_between_centrality_result[i]);
		}
		printf("\nResults computed and printed in (%s)\n",output_path);
	}

}

int main( int argc, char *argv[] )
{
    int i;
    int rank;
    int size;
    MPI_Status    status;
    char str_message[100];

    if(argc != 4){
       	printf("\nWrong Arguments..\n");
       	printf("Program will terminate\n");
       	exit(1) ;
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    //Start ZIYAD_CHANGE
	int number_of_threads = atoi(argv[1]) ; //ZIYAD_COMMENT: Take the number of threads that master thread will create
	string graph_path =  argv[2] ; 			//ZIYAD_COMMENT: Take the graph path
	char* result_path = argv[3] ;			//ZIYAD_COMMENT: Take the output path
	run_parallel_brandes(graph_path, result_path, number_of_threads);
	//End ZIYAD_CHANGE

    MPI_Finalize();
    return 0;
}
