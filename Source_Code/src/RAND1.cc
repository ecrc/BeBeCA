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

void run_RAND1(
        string  graph_path,
        char  output_path[],
        int     sample_size
        )
{
	sgraph_t comp;
	prepare_sgraph(graph_path,comp) ;

	vector<double> RAND1_between_centrality_result ;

	printf("===================================================================================") ;
	printf("\nComputation is Running for RAND1 and sample size = %d\n",sample_size) ;

	timer tm ;
	tm.start();
	RAND1(comp, RAND1_between_centrality_result,sample_size);
	tm.stop();

	std::string graph_name = extract_graph_name(graph_path) ;
	printf("\n@STAT	Alg#	RAND1	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), sample_size) ;

	int graph_size = comp.size() ;
	for(int i = 0 ; i<RAND1_between_centrality_result.size() ; i++){
		RAND1_between_centrality_result[i] = (graph_size/sample_size)*RAND1_between_centrality_result[i];
	}

	FILE *RAND1Output ;
	RAND1Output = fopen(output_path,"w");
	for(int i = 0 ; i<RAND1_between_centrality_result.size() ; i++){
		fprintf(RAND1Output,"%d:	     %f\n",i,RAND1_between_centrality_result[i]);
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

	int sampleSize = atoi(argv[1]) ; 		//ZIYAD_COMMENT: Take the sample size
	string graph_path =  argv[2] ; 			//ZIYAD_COMMENT: Take the graph path
	char* result_path = argv[3] ;			//ZIYAD_COMMENT: Take the output path
	run_RAND1(graph_path, result_path, sampleSize);

	return 0;
}
