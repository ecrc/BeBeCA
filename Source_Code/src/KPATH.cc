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

void run_KPATH(
        string  graph_path,
        char  output_path[],
        int     k_path
        )
{
	sgraph_t comp;
	prepare_sgraph(graph_path,comp) ;
	
	vector<double> KPATH_between_centrality_result ;

	printf("===================================================================================") ;
	printf("\nComputation is Running for k-centralities with k = %d \n",k_path) ;

	timer tm ;
	tm.start();
	limited_traverse(comp, KPATH_between_centrality_result,k_path);
	tm.stop();

	std::string graph_name = extract_graph_name(graph_path) ;
	printf("\n@STAT	Alg#	KPATH	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), comp.size()) ;	

	FILE *KPATHOutput ;
	KPATHOutput = fopen(output_path,"w");
	for(int i = 0 ; i<KPATH_between_centrality_result.size() ; i++){
		fprintf(KPATHOutput,"%d:	     %0.12f\n",i,KPATH_between_centrality_result[i]);
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

	int k_path = atoi(argv[1]) ; 
	string graph_path =  argv[2] ; 		
	char* result_path = argv[3] ;	
	run_KPATH(graph_path, result_path, k_path);

    return 0;
}
