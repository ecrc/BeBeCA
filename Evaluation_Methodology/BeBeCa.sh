#!/bin/bash

exactGraph=$1
approxGraph=$2
outputPath=$3

if [ -z "$exactGraph" ] || [ -z "$approxGraph" ] || [ -z "$outputPath" ]
then
	echo "Missing Arguments !"
	echo ""
else
	outputPath=$3
	exactGraphSize=$(wc -l $exactGraph | awk '{print $1}')
	approxGraphSize=$(wc -l $approxGraph | awk '{print $1}')
	if [ "$exactGraphSize" = "$approxGraphSize" ]
	then
		source ./Evaluation_Scripts/Evaluate.sh $1 $2 | tee -a $outputPath
	else
		echo "The number of nodes aren't the same and we can't evaluate!"
		echo ""
	fi
fi


