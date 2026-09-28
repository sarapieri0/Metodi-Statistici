#ifndef TRACK_UTILS_H
#define TRACK_UTILS_H

#include <vector>
using namespace std;

vector<int> single_double_hit(double *z_double, vector<double>& z, vector<double>& x, vector<double>& e_dep, int indx, vector<double>& mu_sgl, vector<double>& sigma_sgl);

vector<int> gen_double_hit(int z_counts[], double z_coords[], vector<double>& z, vector<double>& x, vector<double>& e_dep, vector<double>& mu_sgl, vector<double>& sigma_sgl);



#endif