#include <iostream>
#include <stdio.h>
#include <fstream>
#include <list>
#include <algorithm>
#include <cmath>
#include "graph_t.h"
#include "bc.h"
#include "utility.h"
#include <ctime>

using namespace std;


void run_parallel_brandes(
        string  graph_path,
        char  output_path[],
        int     sample_size
        )
{
	//ZIYAD_COMMENT: Note that all part related to the serial computation is commented and it can be used for verifying the results only
	sgraph_t comp;
	prepare_subgraph(graph_path,comp) ;

	//ZIYAD_COMMENT: These two vector to store the results
	//vector<double> serialBetweenCentralityResult ;
	vector<double> parallel_between_centrality_result ;

	//ZIYAD_COMMENT: to compute between centality in serial of parallel fashion depend on how many nodes involved
	printf("===================================================================================") ;
	printf("\nComputation is Running for RAND1 and sample size = %d\n",sample_size) ;

	timer tm ;
	tm.start();
	parallel_brandes(comp, parallel_between_centrality_result,sample_size);
	tm.stop();

	std::string graph_name = extract_graph_name(graph_path) ;
	printf("\n@STAT	Alg#	RAND1	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), sample_size) ;

	int graph_size = comp.size() ;
	for(int i = 0 ; i<parallel_between_centrality_result.size() ; i++){
		parallel_between_centrality_result[i] = (graph_size/sample_size)*parallel_between_centrality_result[i];
	}

	//ZIYAD_COMMENT:Store results of both serial and parallel implementation for comparisons
	FILE *parallelBrandesOutput ;
	parallelBrandesOutput = fopen(output_path,"w");
	for(int i = 0 ; i<parallel_between_centrality_result.size() ; i++){
		fprintf(parallelBrandesOutput,"%d:	     %f\n",i,parallel_between_centrality_result[i]);
	}
	printf("\nResults computed and printed in (%s)\n",output_path);

}

int main( int argc, char *argv[] )
{
	int i;
	int size;
	char str_message[100];

	if(argc != 4){
	printf("\nWrong Arguments..\n");
	printf("Program will terminate\n");
	exit(1) ;
	}
	//Start ZIYAD_CHANGE
	int number_of_threads = atoi(argv[1]) ; //ZIYAD_COMMENT: Take the number of threads that master thread will create
	string graph_path =  argv[2] ; 			//ZIYAD_COMMENT: Take the graph path
	char* result_path = argv[3] ;			//ZIYAD_COMMENT: Take the output path
	run_parallel_brandes(graph_path, result_path, number_of_threads);
	//End ZIYAD_CHANGE

	return 0;
}
