#include "iter_info_t.h"
#include "utility.h"

void iter_info_t::init_all(size_t N)
{
    fill_vec<vector<node_id_t> >(P, N, vector<node_id_t>());
    fill_vec<int>(sigma_vec, N, 0);
    fill_vec<int>(dist_vec, N, -1);

    S.clear();
}
