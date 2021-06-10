#ifndef hcms_hpp
#define hcms_hpp

#include <vector>
#include "anoedgelocal.hpp"
using namespace std;

class Hcms {
    public:
        Hcms(int r, int b);
        Hcms(int r, int b, int d);

        void insert(int a, int b, double weight);
        void clear();
        void decay(double decay_factor);

        double getAnoedgeglobalScore(string algorithm, int src, int dst);

        void intializeDenseSubmatrices();
        void decay(double decay_factor, bool flag);
        double getAnoedgelocalScore(string algorithm, int src, int dst);

        double getAnographScore(string algorithm);

        double getAnographKScore(string algorithm, int K);

    private:
        int num_rows;
        int num_buckets;
        int num_dense_submatrices;

        vector<int> hash_a, hash_b;
        vector<vector<vector<double>>> count;
        vector<vector<Submatrix>> densest_matrices;

        int hash(int elem, int i);
        double getCount(int a, int b);
};

#endif