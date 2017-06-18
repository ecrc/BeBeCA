/* 
 * File:   iter_info_t.h
 * Author: fuad
 *
 * Created on November 24, 2014, 4:09 PM
 */


#ifndef ITER_INFO_T_H
#define	ITER_INFO_T_H

#include <vector>
#include "types.h"
#include "utility.h"

using namespace std;

struct iter_info_t {
    vector<vector<node_id_t> >       P;
    vector<int>                      sigma_vec;
    vector<int>                      dist_vec;
    vector<double>                   delta_vec;

    vector<node_id_t>                S;
	
	double 							 sum_of_dependancies;

    void init_all(size_t N);
};

inline void iter_info_t::init_all(size_t N)
{
    fill_vec<vector<node_id_t> >(P, N, vector<node_id_t>());
    fill_vec<int>(sigma_vec, N, 0);
    fill_vec<int>(dist_vec, N, -1);

    S.clear();
}

#endif	/* ITER_INFO_T_H */

