© 2017 KAUST - InfoCloud group
Ziyad Al-Ghamdi  <Ziyad.AlGhamdi@kaust.edu.sa>
Fuad Jamour <fuad.jamour@kaust.edu.sa>

=============================================

BeBeCa (The evaluation framework)

=============================================

This readme file contains information on how to use the BeBeCa framework. 

----------------------------------------------
-                  Contents                  -
----------------------------------------------
Included in this framework are the following files and directories:
  - README                        This document
  - Approximate_Scores/       	  An empty directory where you can place you approximate
								  betweenness centrality scores. 
  - Exact_Scores/				  Contains all exact betweenness centrality scores. The 
								  normalized betweenness scores are available in the 
								  directory Exact_Scores/Normalized_Scores. 
  - Evaluation_Results/           The folder that will contain the evaluations results files
								  after executing BeBeCa.sh
  - Evaluation_Scripts/			  Contains the scripts that are used by BeBeCa.sh. 
  - Graphs/						  Containts all the datasets presented in the paper. 
                                  

----------------------------------------------
-            How to run BeBeCa.sh            -
----------------------------------------------
The following evaluation metrics:
	1- Average Error
	2- Maximum Error
	3- Top 1% Hit
	4- Kindall-tau Distance
Evaluation_Methodology/BeBeCa.sh takes as arguments: 
	1- The exact betweenness centrality scores file path. 
	2- The approximate betweenness centrality scores file path.
	3- The output file name that will contain the evaluation metrics. 
The evaluation metrics could be obtained by the following steps:-
	1- Put the exact scores in the "Exact_Scores/" folder
	2- Put the approximated scores in the "Approximate_Scores/" folder
	3- To get the evaluation metrics, run the following command:-
		format:> ./BeBeCa.sh <exact_betweenness_scores> <approximate_betweenness_scores> <output_path>
		e.g.  :> ./BeBeCa.sh Exact_Scores/Normalized_Scores/ca-GrQc.txt Approximate_Scores/RAND1_ca-GrQc.txt RAND1_Evaluate_ca-GrQc.txt
	4- The output will be printed in the terminal and appended to the output path 
		given in the above command inside the folder "Evaluation_Results/"
You can copy and paste the example shown in step 3 to see how it works. 
The runtime is printed when you run any of our implementations (Section 5.1 in the paper).
The specific nodes evaluation are done manually (Section 5.6 in the paper).

----------------------------------------------
-             Evaluation_Scripts/            -
----------------------------------------------
The "Evaluation_Scripts/" directory contains the scripts used by BeBeCa.sh to get the 
evaluation metrics. It containts the following files:
	1- Evaluate.sh
	2- Kendall.py
	3- TopHit.jar
Evaluate.sh is used to get the average and the maximum error between the exact and the
approximate betweenness centrality values. Kendall.py is used to get the kendall-tau
distance and TopHit.jar is used to identify and calculate the top 1% hit of the top 
nodes.

----------------------------------------------
-               Exact_Scores/                -
----------------------------------------------
In the directory "Exact_Scores/" you will find the exact betweenness (Golden Standard) 
centrality values for the graphs provided in the folder "Graphs/"
The exact betweenness centrality scores are computed using the source code available in
"BeBeCa/Source_Code/Parallel_Brandes/"
The output format of the Golden Standard is:-
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

The normalized betweenness centrality values are available in the directory
"Exact_Scores/Normalized_Scores/"	
							
----------------------------------------------
-                  Graphs/                   -
----------------------------------------------
In the directory "Graphs/" You will find all the graphs inputs presented in the paper.
All the datasets are downloaded from the following two repositories:
	1- SNAP: https://snap.stanford.edu/data/
	2- KONECT: http://konect.uni-koblenz.de/networks/

All the datets have the following format:
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

Note that for the approximation algorithms available in "BeBeCa/Source_Code/DIAM_RDAVG_RAND2", 
you need to remove the first line (that containts |V| and |E|). See README.txt at 
"BeBeCa/Source_Code/DIAM_RDAVG_RAND2" for further details.
