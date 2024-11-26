#include <iostream>
#include <algorithm>

#include "hcms.hpp"
#include "anoedgeglobal.hpp"
#include "anoedgelocal.hpp"
#include "anograph.hpp"
#include "anographk.hpp"
#include "utils.hpp"

using namespace std;

Hcms::Hcms(int r, int b) {
    num_rows = r;
    num_buckets = b;

    hash_a.resize(num_rows);
    hash_b.resize(num_rows);
    for (int i = 0; i < num_rows; i++) {
        hash_a[i] = rand() % (num_buckets - 1) + 1;
        hash_b[i] = rand() % num_buckets;
    }
    this->clear();
}

Hcms::Hcms(int r, int b, int d) {
    num_rows = r;
    num_buckets = b;
    num_dense_submatrices = d;

    hash_a.resize(num_rows);
    hash_b.resize(num_rows);
    for (int i = 0; i < num_rows; i++) {
        hash_a[i] = rand() % (num_buckets - 1) + 1;
        hash_b[i] = rand() % num_buckets;
    }
    this->clear();
}

int Hcms::hash(int elem, int i) {
    int resid = (elem * hash_a[i] + hash_b[i]) % num_buckets;
    return resid + (resid < 0 ? num_buckets : 0);
}

void Hcms::insert(int a, int b, double weight) {
    for (int i = 0; i < num_rows; i++) {
        int a_bucket = hash(a, i);
        int b_bucket = hash(b, i);
        count[i][a_bucket][b_bucket] += weight;
    }
}

double Hcms::getCount(int a, int b) {
    double min_count = numeric_limits<double>::max();
    for (int i = 0; i < num_rows; i++) {
        int a_bucket = hash(a, i);
        int b_bucket = hash(b, i);
        min_count = MIN(min_count, count[i][a_bucket][b_bucket]);
    }
    return min_count;
}

void Hcms::decay(double decay_factor) {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_buckets; j++) {
            for (int k = 0; k < num_buckets; k++) {
                count[i][j][k] = count[i][j][k]*decay_factor;
            }
        }
    }
}

double Hcms::getAnoedgeglobalScore(string algorithm, int src, int dst) {
    double min_dsubgraph = numeric_limits<double>::max();
    for (int i = 0; i < num_rows; i++) {
        int src_bucket = hash(src, i);
        int dst_bucket = hash(dst, i);
        double cur_dsubgraph = AnoedgeGlobal::getAnoedgeglobalDensity(count[i], src_bucket, dst_bucket);
        min_dsubgraph = MIN(min_dsubgraph, cur_dsubgraph);
    }
    return min_dsubgraph;
}

void Hcms::intializeDenseSubmatrices() {
    for (int i = 0; i < num_rows; i++) {
        vector<Submatrix> cur_dense_submatrix;
        vector<pair<double, pair<int, int>>> flat_counts;
        for (int j = 0; j < num_buckets; j++) {
            for (int k = 0; k < num_buckets; k++) {
                flat_counts.push_back({count[i][j][k], {j, k}});
            }
        }
        sort(flat_counts.begin(), flat_counts.end(), greater<pair<double, pair<int, int>>>());
        for (int j = 0; j < num_dense_submatrices; j++) {
            Submatrix initial_submatrix(j, j, 0.0);
            cur_dense_submatrix.push_back(initial_submatrix);
        }
        densest_matrices.push_back(cur_dense_submatrix);
    }
}

double Hcms::getAnoedgelocalScore(string algorithm, int src, int dst) {
    double min_dsubgraph = numeric_limits<double>::max();
    for (int i = 0; i < num_rows; i++) {
        int src_bucket = hash(src, i);
        int dst_bucket = hash(dst, i);

        for (int j = 0; j < num_dense_submatrices; j++) {
            if (densest_matrices[i][j].checkAndAdd(src_bucket, dst_bucket, count[i])) {
                bool flag = true;
                int del_cnt = -1;
                while (flag) {
                    flag = densest_matrices[i][j].checkAndDel(count[i]);
                    del_cnt++;
                }
            }
        }

        double cur_dsubgraph = 0.0;
        for (int j = 0; j < num_dense_submatrices; j++) {
            double cur_likelihood = densest_matrices[i][j].getLikelihoodScore(src_bucket, dst_bucket, count[i]);
            cur_dsubgraph = cur_dsubgraph + cur_likelihood;
        }
        min_dsubgraph = MIN(min_dsubgraph, cur_dsubgraph);
    }
    return min_dsubgraph;
}

void Hcms::decay(double decay_factor, bool flag) {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_buckets; j++) {
            for (int k = 0; k < num_buckets; k++) {
                count[i][j][k] = count[i][j][k]*decay_factor;
            }
        }
    }
    if (flag) {
        for (int i = 0; i < num_rows; i++) {
            for (int j = 0; j < num_dense_submatrices; j++) {
                densest_matrices[i][j].decay(decay_factor);
            }
        }
    }
}

double Hcms::getAnographScore(string algorithm) {
    double min_dsubgraph = numeric_limits<double>::max();
    for (int i = 0; i < num_rows; i++) {
        double cur_dsubgraph = Anograph::getAnographDensity(count[i]);
        min_dsubgraph = MIN(min_dsubgraph, cur_dsubgraph);
    }
    return min_dsubgraph;
}

double Hcms::getAnographKScore(string algorithm, int K) {
    double min_dsubgraph = numeric_limits<double>::max();
    for (int i = 0; i < num_rows; i++) {
        double cur_dsubgraph = AnographK::getAnographKDensity(count[i], K);
        min_dsubgraph = MIN(min_dsubgraph, cur_dsubgraph);
    }
    return min_dsubgraph;
}

void Hcms::clear() {
    count = vector<vector<vector<double>>>(num_rows, vector<vector<double>>(num_buckets, vector<double>(num_buckets, 0.0)));
}
