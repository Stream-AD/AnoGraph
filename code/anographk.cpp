#include <iostream>
#include <cmath>
#include <queue>
#include <algorithm>

#include "anographk.hpp"
#include "hcms.hpp"
#include "utils.hpp"

using namespace std;

AnographK::AnographK(string _algorithm, string _dataset_name, int _time_window, int _edge_threshold, int _rows, int _buckets, int _topK) {
	algorithm = _algorithm;
	dataset_name = _dataset_name;
	time_window = _time_window;
	edge_threshold = _edge_threshold;
	rows = _rows;
	buckets = _buckets;
    topK = _topK;

    ReadUtils::loadGraphData(graphs, labels, dataset_name, time_window, edge_threshold);
}

vector<double> AnographK::getScores() {
    vector<double> scores;
    int num_graphs = graphs.size();

    for (int i = 0; i < num_graphs; i++) {
        vector<int> src = graphs[i].first;
        vector<int> dst = graphs[i].second;

        Hcms cur_count(rows, buckets);

        int num_edges = src.size();
        for (int j = 0; j < num_edges; j++) {
            cur_count.insert(src[j], dst[j], 1);
        }
        double score = cur_count.getAnographKScore(algorithm, topK);
        scores.push_back(score);
    }
    return scores;
}

void AnographK::run() {
	clock_t start_time = clock();
	vector<double> scores = getScores();
	double total_time = ((double)(clock() - start_time)) / CLOCKS_PER_SEC;

	string output_base_path = RESULT_BASE_PATH + algorithm + "_" + dataset_name + "_" + to_string(time_window) + "_" + to_string(edge_threshold);
	WriteUtils::writeScoresAndLabels(scores, labels, output_base_path + SCORE_FILE_SUFFIX);
	WriteUtils::writeTime(total_time, scores.size(), output_base_path + TIME_FILE_SUFFIX);
}

double AnographK::getSubgraphDensity(vector<vector<double>>& mat, int src, int dst) {
    int num_rows = mat.size();
    int num_cols = mat[0].size();

    bool row_flag[num_rows];
    bool col_flag[num_cols];

    double row_slice_sum[num_rows];
    double col_slice_sum[num_cols];

    for (int i = 0; i < num_rows; i++) {
        row_flag[i] = false;
        row_slice_sum[i] = mat[i][dst];
    }
    for (int i = 0; i < num_cols; i++) {
        col_flag[i] = false;
        col_slice_sum[i] = mat[src][i];
    }

    row_flag[src] = true;
    col_flag[dst] = true;
    row_slice_sum[src] = mat[src][dst];
    col_slice_sum[dst] = mat[src][dst];

    pair<int, double> max_row = {-1, -1.0};
    for (int i = 0; i < num_rows; i++) {
        if (!row_flag[i] && (row_slice_sum[i] >= max_row.second)) {
            max_row = {i, row_slice_sum[i]};
        }
    }

    pair<int, double> max_col = {-1, -1.0};
    for (int i = 0; i < num_cols; i++) {
        if (!col_flag[i] && (col_slice_sum[i] >= max_col.second)) {
            max_col = {i, col_slice_sum[i]};
        }
    }

    int marked_rows = 1;
    int marked_cols = 1;

    double cur_mat_sum = mat[src][dst];
    double output = cur_mat_sum/sqrt(marked_rows*marked_cols);

    int ctr = num_rows + num_cols - 2;
    while (ctr--) {
        if (max_row.second >= max_col.second) {
            row_flag[max_row.first] = true;
            marked_rows++;

            max_col = {-1, -1.0};
            for (int i = 0; i < num_cols; i++) {
                if (col_flag[i]) {
                    cur_mat_sum = cur_mat_sum + mat[max_row.first][i];
                } else {
                    col_slice_sum[i] = col_slice_sum[i] + mat[max_row.first][i];
                    if (col_slice_sum[i] >= max_col.second) {
                        max_col = {i, col_slice_sum[i]};
                    }
                }
            }

            max_row = {-1, -1.0};
            for (int i = 0; i < num_rows; i++) {
                if (!row_flag[i] && (row_slice_sum[i] >= max_row.second)) {
                    max_row = {i, row_slice_sum[i]};
                }
            }
        } else {
            col_flag[max_col.first] = true;
            marked_cols++;

            max_row = {-1, -1.0};
            for (int i = 0; i < num_rows; i++) {
                if (row_flag[i]) {
                    cur_mat_sum = cur_mat_sum + mat[i][max_col.first];
                } else {
                    row_slice_sum[i] = row_slice_sum[i] + mat[i][max_col.first];
                    if (row_slice_sum[i] >= max_row.second) {
                        max_row = {i, row_slice_sum[i]};
                    }
                }
            }

            max_col = {-1, -1.0};
            for (int i = 0; i < num_cols; i++) {
                if (!col_flag[i] && (col_slice_sum[i] >= max_col.second)) {
                    max_col = {i, col_slice_sum[i]};
                }
            }
        }
        output = MAX(output, cur_mat_sum/sqrt(marked_rows*marked_cols));
    }
    return output;
}

double AnographK::getAnographKDensity(vector<vector<double>>& mat, int K) {
    int num_subgraphs = K;
    int num_rows = mat.size();
    int num_cols = mat[0].size();

    vector<pair<double, pair<int, int>>> flat_mat;
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            flat_mat.push_back({mat[i][j], {i, j}});
        }
    }
    sort(flat_mat.begin(), flat_mat.end(), greater<pair<double, pair<int, int>>>());

    double output_density = 0.0;
    for (int idx = 0; idx < num_subgraphs; idx++) {
        output_density = MAX(output_density, getSubgraphDensity(mat, flat_mat[idx].second.first, flat_mat[idx].second.second));
    }
    return output_density;
}