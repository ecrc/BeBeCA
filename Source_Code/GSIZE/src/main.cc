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


/*
 *
 */
void run_parallel_brandes(
        string  graph_path,
        char  output_path[],
		int 	chosen_vertex,
        double     c_constant
        )
{
	//ZIYAD_COMMENT: Note that all part related to the serial computation is commented and it can be used for verifying the results only

	component_t comp;
	prepare_subgraph(graph_path,comp) ;

	//ZIYAD_COMMENT: These two vector to store the results
	//vector<double> serialBetweenCentralityResult ;
	vector<double> parallel_between_centrality_result ;

	printf("===================================================================================") ;
	printf("\nComputation is Running with vertex# %d chosen and c = %f\n",chosen_vertex, c_constant) ;


	timer tm ;
	tm.start();
	int adaptive_sample_size = parallel_brandes(comp, parallel_between_centrality_result,chosen_vertex, c_constant);
	tm.stop();

	std::string graph_name = extract_graph_name(graph_path) ;
	printf("\n@STAT	alg#	GSIZE	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), adaptive_sample_size) ;
	
	int graph_size = comp.subgraph.size() ;
	for(int i = 0 ; i<parallel_between_centrality_result.size() ; i++){
		parallel_between_centrality_result[i] = (graph_size/adaptive_sample_size)*parallel_between_centrality_result[i];
	}
	
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

    if(argc != 5){
       	printf("\nWrong Arguments..\n");
       	printf("Program will terminate\n");
       	exit(1) ;
    }
    //Start ZIYAD_CHANGE
	int chosen_vertex = atoi(argv[1]) ; //ZIYAD_COMMENT: Take the number of threads that master thread will create
	double c_constant = atof(argv[2]) ;
	string graph_path =  argv[3] ; 			//ZIYAD_COMMENT: Take the graph path
	char* result_path = argv[4] ;			//ZIYAD_COMMENT: Take the output path
	run_parallel_brandes(graph_path, result_path, chosen_vertex, c_constant);
	//End ZIYAD_CHANGE
    return 0;
}
