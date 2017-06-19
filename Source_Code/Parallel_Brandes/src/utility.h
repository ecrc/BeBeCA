/* 
 * File:   utility.h
 * Author: fuad
 *
 * Created on March 2, 2014, 10:22 AM
 */

#ifndef UTILITY_H
#define	UTILITY_H

#include <sys/time.h>  
#include <stdio.h> 
#include <algorithm>
#include <numeric>
#include <math.h>
#include <iostream>
#include "graph_t.h"

using namespace std;

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

inline string extract_graph_name(string path)
{
    string out;
    int pos = path.rfind("/") + 1;
    int length = path.length() - pos + 1;
    out = path.substr(pos, length);
    return out;
}

template <typename T>
void fill_vec(vector<T>& vec, size_t N, T val)
{
	vec.resize(N);
    fill(vec.begin(), vec.end(), val);
}


#endif	/* UTILITY_H */

