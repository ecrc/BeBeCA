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
        int     k_path
        )
{
	//ZIYAD_COMMENT: Note that all part related to the serial computation is commented and it can be used for verifying the results only

	sgraph_t comp;
	prepare_subgraph(graph_path,comp) ;

	//ZIYAD_COMMENT: These two vector to store the results
	//vector<double> serialBetweenCentralityResult ;
	vector<double> parallel_between_centrality_result ;


	printf("===================================================================================") ;
	printf("\nComputation is Running on Ego Networks\n") ;

	timer tm ;
	tm.start();
	parallel_brandes(comp, parallel_between_centrality_result,k_path);
	tm.stop();

	std::string graph_name = extract_graph_name(graph_path) ;
	printf("\n@STAT	Alg#	EGO	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), comp.size()) ;	

	int graph_size = comp.size() ;
	//ZIYAD_COMMENT:Store results of both serial and parallel implementation for comparisons
	FILE *parallelBrandesOutput ;
	parallelBrandesOutput = fopen(output_path,"w");
	for(int i = 0 ; i<parallel_between_centrality_result.size() ; i++){
		fprintf(parallelBrandesOutput,"%d:	     %0.12f\n",i,parallel_between_centrality_result[i]);
	}
	printf("\nResults computed and printed in (%s)\n",output_path);
}

int main( int argc, char *argv[] )
{
	int i;
	int size;
	char str_message[100];

	if(argc != 3){
	printf("\nWrong Arguments..\n");
	printf("Program will terminate\n");
	exit(1) ;
	}


	//Start ZIYAD_CHANGE
	int ego = 2 ;
	string graph_path =  argv[1] ; 			//ZIYAD_COMMENT: Take the graph path
	char* result_path = argv[2] ;			//ZIYAD_COMMENT: Take the output path
	run_parallel_brandes(graph_path, result_path, ego);
	//End ZIYAD_CHANGE

    return 0;
}
