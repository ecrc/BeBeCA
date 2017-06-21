#include <iostream>
#include <stdio.h>
#include <fstream>
#include <list>
#include <algorithm>
#include <cmath>
#include "graph_t.h"
#include "bc.h"
#include "utility.h"
#include "mpi.h"
#include <ctime>

using namespace std;

void run_parallel_brandes(
        string  graph_path,
        char  output_path[],
        int     num_threads
        )
{
	sgraph_t comp;
	prepare_sgraph(graph_path,comp) ;

	vector<double> parallel_between_centrality_result ;

	int rank, number_of_nodes;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD,&number_of_nodes);

	if(num_threads<0){
		if(rank==0){
			timer tm ;
			int number_of_vercies = (num_threads*-1) ;
			printf("Traverse From %d Random Vertices for Graph of Size %d \n",number_of_vercies, comp.size()) ;
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
		printf("\n@STAT	Alg#	Brandes	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), comp.size()) ;	
	}

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

	int number_of_threads = atoi(argv[1]) ; 
	string graph_path =  argv[2] ; 			
	char* result_path = argv[3] ;			
	run_parallel_brandes(graph_path, result_path, number_of_threads);

    MPI_Finalize();
    return 0;
}
