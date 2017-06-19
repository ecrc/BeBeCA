© 2017 KAUST - InfoCloud group
Ziyad Al-Ghamdi  <amin.allam@kaust.edu.sa>
Fuad Jamour <fuad.jamour@kaust.edu.sa>

=============================================

BeBeCa (The source code)

=============================================

This readme file contains information on how to compile and execute
the approximate algorithms available in "Source_Code/"

----------------------------------------------
-             The Input Format               -
----------------------------------------------
1- The format of graph inputs for Parallel_Brandes, RAND1, GSIZE, KPATH and EGO:
	First line has the number of nodes and edges, and the remaining lines are edges list,
	each represented by two nodes IDs.
	
	Format:-
	|V| |E|
	Ew Ey
	Ex Ez
	.  .
	.  .
	
	e.g.:-
	4158 13428
	0 1
	0 2
	. .
	. .

2- The format of graph inputs for RAND2, DIAM and ABRA:
	The input file should contain only the edge list without the number of nodes and edges.
	
	Format:-
	Ew Ey
	Ex Ez
	.  .
	.  .
	
	e.g.:-
	0 1
	0 2
	. .
	. .
	
	The following command will remove the first line from the input file:-
	sed 1d graph_input > graph_input_newformat

3- The output format is the same for all implementations:
	Each line will have the node ID along with its betweenness centrality score:-
	
	Format:-
	Vx:			BCx
	Vy:			BCy
	Vz:			BCz
	.    		.
	.    		.
	
	e.g.:-
	0:           23552.792335
	1:           4796.951968
	2:           0.000000


----------------------------------------------
-  How to compile and run each algorithm ?   -
----------------------------------------------
-----------------------
Parallel_Brandes:-
	- Notes:
		1- In order to compile and run Parallel_Brandes, your machine should support "Open MPI" library
	- How to compile? 
		From the source directory, the following command is used for compiling:-
			mpic++ -o ./Parallel_Brandes -std=c++11 *.cc
	- How to run? 
		From the executable directory, the following command is used for execution:-
			format: mpirun -np <number_of_processes> ./Parallel_Brandes <number_of_threads> <graph_input_path> <scores_output_path>
			e.g.  : mpirun -np 2 ./Parallel_Brandes 4 ca-GrQc.lcc.net ca-GrQc_Scores.txt
-----------------------
RAND1:-
	- Notes:
	- How to compile? 
		From the source directory, the following command is used for compiling:-
			g++ -std=c++11 -o RAND1 *.cc
	- How to run? 
		From the executable directory, the following command is used for execution:-
			format: ./RAND1 <number_of_nodes> <graph_input_path> <scores_output_path>
			e.g.  : ./RAND1 200 ca-GrQc.lcc.net ca-GrQc.lcc_Scores.txt
-----------------------
GSIZE:-
	- Notes:
	- How to compile? 
		From the source directory, the following command is used for compiling:-
			g++ -std=c++11 -o GSIZE *.cc
	- How to run? 
		From the executable directory, the following command is used for execution:-
			format: ./GSIZE <node_ID> <c_constant> <graph_input_path> <scores_output_path>
			e.g.  : ./GSIZE 1022 5 ca-GrQc.lcc.net ca-GrQc_Scors.txt
-----------------------
KPATH:-
	- Notes:
	- How to compile? 
		From the source directory, the following command is used for compiling:-
			g++ -std=c++11 -o KPATH *.cc
	- How to run? 
		From the executable directory, the following command is used for execution:-
			format: ./KPATH <number_of_hops> <graph_input_path> <scores_output_path>
			e.g.  : ./KPATH 4 ca-GrQc.lcc.net ca-GrQc_Scores.txt
-----------------------
EGO:-
	- Notes:
	- How to compile? 
		From the source directory, the following command is used for compiling:-
			g++ -std=c++11 -o EGO *.cc
	- How to run? 
		From the executable directory, the following command is used for execution:-
			format: ./EGO <graph_input_path> <scores_output_path>
			e.g.  : ./EGO ca-GrQc.lcc.net ca-GrQc_Scores.txt
-----------------------
RAND2, DIAM and ABRA:-
	- Notes:
		1- In order to compile and run Parallel_Brandes, your machine should support "NetworKit" library
		2- In the compilation, I'm showing an example for paths in my machine. It should be change to yours.
		3- When you run, you should specify which algorithm you want to run:-
			1- Exact betweenness
			2- DIAM
			3- ABRA
			4- RAND2
		4- The format of the input is different. Please refer to the beginning of the READ_ME.txt
	- How to compile? 
		From the source directory, the following command is used for compiling:-
			format: g++ NetworKitApps.cpp -o <executable_name> -std=c++11 -L<path_to_repository> -I<path_to_include> -lNetworKit -fopenmp -lnlopt
			e.g.  : g++ NetworKitApps.cpp -o ApproxKit -std=c++11 -L/home/ziyad/NetworKit/ -I/home/ziyad/NetworKit/include/ -lNetworKit -fopenmp -lnlopt
	- How to run? 
		To run DIAM and ABRA:
			From the executable directory, the following command is used for execution:-
				format: ./ApproxKit <2_or_3> <graph_input_path> <scores_output_path> <epsilon> <delta>
				e.g.  : ./ApproxKit 2 ca-GrQc.lcc.net ca-GrQc_Scores.txt 0.01 0.1
		To run RAND2:
			From the executable directory, the following command is used for execution:-
				format: ./ApproxKit 4 <graph_input_path> <scores_output_path> <number_of_nodes> 0
				e.g.  : ./ApproxKit 4 ca-GrQc.lcc.net ca-GrQc_Scores.txt 900 0
-----------------------
	
