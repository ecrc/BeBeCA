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
using namespace std;

struct iter_info_t {
    vector<vector<node_id_t> >       P;
    vector<int>                      sigma_vec;
    vector<int>                      dist_vec;
    vector<double>                   delta_vec;

    vector<node_id_t>                S;
    
    void init_all(size_t N);
};


#endif	/* ITER_INFO_T_H */

