#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include <unistd.h>

#include <NetworKit/centrality/Betweenness.h>
#include <NetworKit/centrality/ApproxBetweenness.h>
#include <NetworKit/centrality/ApproxBetweenness2.h>
#include <NetworKit/centrality/ApproxBetweennessRade.h>
#include <NetworKit/graph/Graph.h>
#include <NetworKit/io/EdgeListReader.h>
#include <NetworKit/Globals.h>

struct timer {
	timeval i_start_, i_stop_;
	void start() {
		gettimeofday(&i_start_, NULL);
	}

	void stop() {
		gettimeofday(&i_stop_, NULL);
	}

	double interval() {
		double t1 = i_start_.tv_sec + i_start_.tv_usec/1e6;
		double t2 = i_stop_.tv_sec + i_stop_.tv_usec/1e6;
		return (t2-t1);
	}
};

std::string extract_graph_name(std::string path)
{
    std::string out;
    int pos = path.rfind("/") + 1;
    int length = path.length() - pos + 1;
    out = path.substr(pos, length);
    return out;
}

int main(int argc, char* argv[]) {

	int THE_CHOSEN_ALGORITHM = 0 ;
	int EXACT_BETWEENNESS = 1 ;
	int DIAM = 2 ;
	int ABRA = 3 ;
	int RAND2 = 4 ;

	if(argc != 6){
		printf("Wrong Number of Arguments..\n");
		printf("Program will terminate\n");
		exit(1) ;
	}

	int algorithm = atoi(argv[1]) ;
	if(algorithm==EXACT_BETWEENNESS) {
		printf("You have chosen to compute exact betweenness using Brandes' algorithm \n");
		THE_CHOSEN_ALGORITHM = EXACT_BETWEENNESS ;
	}
	else if(algorithm==DIAM) {
		printf("You have chosen to approximate betweenness using DIAM algorithm \n");
		THE_CHOSEN_ALGORITHM = DIAM ;
	}
	else if(algorithm==ABRA) {
		printf("You have chosen to approximate betweenness using ABRA algorithm \n");
		THE_CHOSEN_ALGORITHM = ABRA ;
	}
	else if(algorithm==RAND2) {
		printf("You have chosen to approximate betweenness using RAND2 algorithm \n");
		THE_CHOSEN_ALGORITHM = RAND2 ;
	}
	else {
		printf("Sorry, you have to choose (1) for exact, (2) for DIAM, (3) for ABRA & (4) for RAND2 \n");
		exit(1) ;
	}
	std::string graph_path =  argv[2] ;
	char* output_path =  argv[3] ;
	double epsilon = atof(argv[4]) ;
	int sample_size = atoi(argv[4]) ;
	double delta = atof(argv[5]) ;

	NetworKit::Graph g ;
	NetworKit::EdgeListReader er(' ', 0, "#", true, false) ;

	g = er.read(graph_path) ;
	printf("Finish reading the graph\n") ;

	printf("The graph consist of Vertices = %u,  Edges = %u\n", g.numberOfNodes(), g.numberOfEdges()) ;

	timer tm ;
	int the_required_sample ;
	std::vector< std::pair< uint64_t, double > > ranking ;
	if(algorithm==EXACT_BETWEENNESS) {
		printf("The algorithm for exact is ready to run\n") ;
		tm.start();
		NetworKit::Betweenness betweenness(g, false, false) ;
		printf("The algorithm is running at the moment ..... \n") ;
		betweenness.run();
		tm.stop() ;
		the_required_sample = g.numberOfNodes() ;
		ranking = betweenness.ranking() ;
		
		std::string graph_name = extract_graph_name(graph_path) ;
		printf("\n@STAT	Alg#	Exact-NetworKit	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), the_required_sample) ;
	}
	else if(algorithm==DIAM) {
		printf("The algorithm (Matteo Riondato & Evgenios M.) is ready to run with Epsilon = %f & Delta = %f\n", epsilon, delta) ;
		tm.start();
		NetworKit::ApproxBetweenness betweenness(g, epsilon, delta) ;
		printf("The algorithm is running at the moment ..... \n") ;
		the_required_sample = betweenness.numberOfSamples() ;
		printf("1#: The required number of samples = %d\n",the_required_sample) ;
		betweenness.run();
		the_required_sample = betweenness.numberOfSamples() ;
		printf("2#: The required number of samples = %d\n",the_required_sample) ;
		tm.stop() ;
		ranking = betweenness.ranking() ;

		std::string graph_name = extract_graph_name(graph_path) ;
		printf("\n@STAT	Alg#	DIAM	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), the_required_sample) ;
	}
	else if(algorithm==ABRA) {
		printf("The algorithm (Matteo Riondato & Eli Upfal) is ready to run with Epsilon = %f & Delta = %f\n", epsilon, delta) ;
		tm.start();
		NetworKit::ApproxBetweennessRade betweenness(g, epsilon, delta, 0, false) ;
		printf("The algorithm is running at the moment ..... \n") ;
		the_required_sample = betweenness.numberOfSamples() ;
		printf("1#: The required number of samples = %d\n",the_required_sample) ;
		betweenness.run();
		the_required_sample = betweenness.numberOfSamples() ;
		printf("2#: The required number of samples = %d\n",the_required_sample) ;
		tm.stop() ;
		the_required_sample = betweenness.numberOfSamples() ;
		ranking = betweenness.ranking() ;

		std::string graph_name = extract_graph_name(graph_path) ;
		printf("\n@STAT	Alg#	ABRA	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), the_required_sample) ;
	}
	else if(algorithm==RAND2) {
		printf("The algorithm (Sanders, Geisberger & Schultes) is ready to run with sample size = %d\n", sample_size) ;
		tm.start();
		NetworKit::ApproxBetweenness2 betweenness(g, sample_size, true, true) ;
		printf("The algorithm is running at the moment ..... \n") ;
		betweenness.run();
		tm.stop() ;
		the_required_sample = sample_size ;
		ranking = betweenness.ranking() ;

		std::string graph_name = extract_graph_name(graph_path) ;
		printf("\n@STAT	Alg#	RAND2	GRAPH#	%s	TIME#	%f	SAMPLES#	%d\n", graph_name.c_str(), tm.interval(), the_required_sample) ;
	}
	else {
		printf("Sorry, you have to choose (1) for exact, (2) for DIAM, (3) for ABRA & (4) for RAND2 \n");
		exit(1) ;
	}

	printf("Betweenness scores are computed\n") ;

	//sort(ranking.begin(), ranking.end()); 											//Sort the results according to the vertices number

	printf("Printing Betweenness scores ..... \n") ;
	FILE *parallelBrandesOutput ;
	parallelBrandesOutput = fopen(output_path,"w");
	for(int i = 0 ; i < ranking.size() ; i++){
		fprintf(parallelBrandesOutput,"%d:	     %0.12f\n", (ranking[i]).first , (ranking[i]).second);
	}
	printf("\nResults computed and printed in (%s)\n",output_path);

    return EXIT_SUCCESS;
}


