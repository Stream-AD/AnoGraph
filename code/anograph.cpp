#include <iostream>
#include <cmath>
#include <algorithm>

#include "anograph.hpp"
#include "hcms.hpp"
#include "utils.hpp"

using namespace std;

Anograph::Anograph(string _algorithm, string _dataset_name, int _time_window, int _edge_threshold, int _rows, int _buckets) {
	algorithm = _algorithm;
	dataset_name = _dataset_name;
	time_window = _time_window;
	edge_threshold = _edge_threshold;
	rows = _rows;
	buckets = _buckets;

    ReadUtils::loadGraphData(graphs, labels, dataset_name, time_window, edge_threshold);
}

vector<double> Anograph::getScores() {
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
        double score = cur_count.getAnographScore(algorithm);
        scores.push_back(score);
    }
    return scores;
}

void Anograph::run() {
	clock_t start_time = clock();
	vector<double> scores = getScores();
	double total_time = ((double)(clock() - start_time)) / CLOCKS_PER_SEC;

	string output_base_path = RESULT_BASE_PATH + algorithm + "_" + dataset_name + "_" + to_string(time_window) + "_" + to_string(edge_threshold);
	WriteUtils::writeScoresAndLabels(scores, labels, output_base_path + SCORE_FILE_SUFFIX);
	WriteUtils::writeTime(total_time, scores.size(), output_base_path + TIME_FILE_SUFFIX);
}

pair<int, double> Anograph::pickMinRow(vector<vector<double>>& mat, bool row_flag[], bool col_flag[]) {
	int num_rows = mat.size();
	int num_cols = mat[0].size();
	pair<int, double> ans = {-1, numeric_limits<double>::max()};
	for (int i = 0; i < num_rows; i++) {
		if (row_flag[i]) {
			double row_sum = 0.0;
			for (int j = 0; j < num_cols; j++) {
				if (col_flag[j]) {
					row_sum = row_sum + mat[i][j];
				}
			}
			if (row_sum < ans.second) {
				ans = {i, row_sum};
			}
		}
	}
	return ans;
}

pair<int, double> Anograph::pickMinCol(vector<vector<double>>& mat, bool row_flag[], bool col_flag[]) {
	int num_rows = mat.size();
	int num_cols = mat[0].size();
	pair<int, double> ans = {-1, numeric_limits<double>::max()};
	for (int i = 0; i < num_cols; i++) {
		if (col_flag[i]) {
			double col_sum = 0.0;
			for (int j = 0; j < num_rows; j++) {
				if (row_flag[j]) {
					col_sum = col_sum + mat[j][i];
				}
			}
			if (col_sum < ans.second) {
				ans = {i, col_sum};
			}
		}
	}
	return ans;
}

double Anograph::getMatrixDensity(vector<vector<double>>& mat, bool row_flag[], bool col_flag[]) {
	int num_rows = mat.size();
	int num_cols = mat[0].size();
	double row_ctr = 0.0;
	double col_ctr = 0.0;
	double ans = 0.0;
	for (int i = 0; i < num_rows; i++) {
		if (row_flag[i]) {
			row_ctr += 1.0;
			for (int j = 0; j < num_cols; j++) {
				if (col_flag[j]) {
					ans = ans + mat[i][j];
				}
			}
		}
	}
	for (int j = 0; j < num_cols; j++) {
		if (col_flag[j]) {
			col_ctr += 1.0;
		}
	}
	if (row_ctr == 0.0 || col_ctr == 0.0) {
		return 0.0;
	}
	return ans/sqrt(row_ctr*col_ctr);
}

double Anograph::getAnographDensity(vector<vector<double>>& mat) {
	int num_rows = mat.size();
	int num_cols = mat[0].size();

	vector<double> output;
	bool row_flag[num_rows], col_flag[num_cols];
	for (int i = 0; i < num_rows; i++) {
		row_flag[i] = true;
	}
	for (int i = 0; i < num_cols; i++) {
		col_flag[i] = true;
	}

	output.push_back(getMatrixDensity(mat, row_flag, col_flag));

	int ctr = num_rows + num_cols;
	while (ctr--) {
		pair<int, double> picked_row = pickMinRow(mat, row_flag, col_flag);
		pair<int, double> picked_col = pickMinCol(mat, row_flag, col_flag);

		if (picked_row.second <= picked_col.second) {
			row_flag[picked_row.first] = false;
			double mat_sum = getMatrixDensity(mat, row_flag, col_flag);
			output.push_back(mat_sum);
		} else {
			col_flag[picked_col.first] = false;
			double mat_sum = getMatrixDensity(mat, row_flag, col_flag);
			output.push_back(mat_sum);
		}
	}
	sort(output.begin(), output.end(), greater<double>());
	return output[0];
}