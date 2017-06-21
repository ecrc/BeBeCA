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

void run_GSIZE(
        string  graph_path,
        char  output_path[],
		int 	chosen_vertex,
        double     c_constant
        )
{
	sgraph_t comp;
	prepare_sgraph(graph_path,comp) ;

	vector<double> GSIZE_between_centrality_result ;

	printf("===================================================================================") ;
	printf("\nComputation is Running with vertex# %d chosen and c = %f\n",chosen_vertex, c_constant) ;


	timer tm ;
	tm.start();
	int adaptive_sample_size = GSIZE(comp, GSIZE_between_centrality_result,chosen_vertex, c_constant);
	tm.stop();

	std::string graph_name = extract_graph_name(graph_path) ;
	printf("\n@STAT	alg#	GSIZE	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), adaptive_sample_size) ;
	
	int graph_size = comp.size() ;
	for(int i = 0 ; i<GSIZE_between_centrality_result.size() ; i++){
		GSIZE_between_centrality_result[i] = (graph_size/adaptive_sample_size)*GSIZE_between_centrality_result[i];
	}
	
	FILE *GSIZEOutput ;
	GSIZEOutput = fopen(output_path,"w");
	for(int i = 0 ; i<GSIZE_between_centrality_result.size() ; i++){
		fprintf(GSIZEOutput,"%d:	     %f\n",i,GSIZE_between_centrality_result[i]);
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

	int chosen_vertex = atoi(argv[1]) ; 	
	double c_constant = atof(argv[2]) ;
	string graph_path =  argv[3] ; 			
	char* result_path = argv[4] ;			
	run_GSIZE(graph_path, result_path, chosen_vertex, c_constant);

    return 0;
}
