

#include "utility.h"
#include <algorithm>
#include <numeric>
#include <math.h>
#include <iostream>

using namespace std;


string extract_graph_name(string path)
{
    string out;
    int pos = path.rfind("/") + 1;
    int length = path.length() - pos + 1;
    out = path.substr(pos, length);
    return out;
}
