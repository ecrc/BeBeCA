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

void run_EGO(
        string  graph_path,
        char  output_path[]
        )
{
	sgraph_t comp;
	prepare_sgraph(graph_path,comp) ;

	vector<double> EGO_between_centrality_result ;

	printf("===================================================================================") ;
	printf("\nComputation is Running on Ego Networks\n") ;

	timer tm ;
	tm.start();
	limited_traverse(comp, EGO_between_centrality_result,2);
	tm.stop();

	std::string graph_name = extract_graph_name(graph_path) ;
	printf("\n@STAT	Alg#	EGO	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), comp.size()) ;	

	int graph_size = comp.size() ;

	FILE *EGOOutput ;
	EGOOutput = fopen(output_path,"w");
	for(int i = 0 ; i<EGO_between_centrality_result.size() ; i++){
		fprintf(EGOOutput,"%d:	     %0.12f\n",i,EGO_between_centrality_result[i]);
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

	string graph_path =  argv[1] ; 	
	char* result_path = argv[2] ;		
	run_EGO(graph_path, result_path);
	
    return 0;
}
