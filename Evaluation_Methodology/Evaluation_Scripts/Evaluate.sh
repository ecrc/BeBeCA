#!/bin/bash

exactGraph=$1
approxGraph=$2
topHit=1/100

if [ -z "$exactGraph" ] || [ -z "$approxGraph" ]
then
	echo "Missing Arguments !"
	echo ""
else
	exactGraphSize=$(wc -l $exactGraph | awk '{print $1}')
	approxGraphSize=$(wc -l $approxGraph | awk '{print $1}')
	if [ "$exactGraphSize" = "$approxGraphSize" ]
	then
		echo "Start Evaluating :........."
		echo ""
	
		#finding the average error
		join <(sort -k1 -n $exactGraph) <(sort -k1 -n $approxGraph) | awk '{print $2" "$3}' | awk '{print $1-$2}' | awk '{if($1<0) print $1*-1 ; else print $1}' | awk '{ sum += $1 } END {print "Average Error = "sum/'$exactGraphSize'}'
	
		#finding maximum error
		join <(sort -k1 -n $exactGraph) <(sort -k1 -n $approxGraph) | awk '{print $2" "$3}' | awk '{print $1-$2}' | awk '{if($1<0) print $1*-1 ; else print $1}' | awk -v max=0 '{if($1>max) max=$1 } END { print "Maximum Error = "max }'
	
		#Prepare files top hit analysis
		sort -k2 -r -n $exactGraph | awk '{print $1}' | sed 's|[^0-9]||g' > temp1.txt
		sort -k2 -r -n $approxGraph | awk '{print $1}' | sed 's|[^0-9]||g' > temp2.txt

		#Prepare files for kendall analysis
		sort -k2 -r -n $exactGraph | awk '{print $1}' | awk '{printf("%d %s\n", NR, $0)}' | sort -k2 -n | awk '{print $1}' | sed 's|[^0-9]||g' > temp4.txt
		sort -k2 -r -n $approxGraph | awk '{print $1}' | awk '{printf("%d %s\n", NR, $0)}' | sort -k2 -n | awk '{print $1}' | sed 's|[^0-9]||g' > temp5.txt
	
		#compute top 1% hit
		topSize=$(($exactGraphSize * $topHit))	
		echo ""
		echo "Top 1% will compare $topSize vertices"
		java -jar Evaluation_Scripts/TopHit.jar temp1.txt temp2.txt $topSize
	
		#compute kendall
		echo ""
		python -W ignore Evaluation_Scripts/Kendall.py
		echo "Kindall Distance :="
		cat temp3.txt
	
		#Clean and remove temporary files
		rm "temp1.txt"
		rm "temp2.txt"
		rm "temp3.txt"
		rm "temp4.txt"
		rm "temp5.txt"

		echo ""
		echo ""
		echo "##### Done #####"
	else
		echo "The number of nodes aren't the same and we can't evaluate!"
		echo ""
	fi
fi







