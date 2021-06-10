#include <iostream>
#include <cmath>
#include <queue>

#include "anoedgelocal.hpp"
#include "hcms.hpp"
#include "utils.hpp"

using namespace std;

AnoedgeLocal::AnoedgeLocal(string _algorithm, string _dataset_name, int _rows, int _buckets, double _decay_factor) {
	algorithm = _algorithm;
	dataset_name = _dataset_name;
	rows = _rows;
	buckets = _buckets;
	decay_factor = _decay_factor;

    ReadUtils::loadEdgeData(src, dst, times, dataset_name);
	ReadUtils::loadEdgeLabelData(labels, dataset_name);
}

vector<double> AnoedgeLocal::getScores() {
    vector<double> scores;
    Hcms count(rows, buckets, 1);

    int last_time = 0;
    int num_records = src.size();

    count.intializeDenseSubmatrices();

    for (int i = 0; i < num_records; i++) {

        if (times[i] - last_time > 0) {
            count.decay(decay_factor, true);
        }

        count.insert(src[i], dst[i], 1.0);

        double score = count.getAnoedgelocalScore(algorithm, src[i], dst[i]);
        scores.push_back(score);
        last_time = times[i];
    }
    return scores;
}

void AnoedgeLocal::run() {
	clock_t start_time = clock();
	vector<double> scores = getScores();
	double total_time = ((double)(clock() - start_time)) / CLOCKS_PER_SEC;

	string output_base_path = RESULT_BASE_PATH + algorithm + "_" + dataset_name;
	WriteUtils::writeScoresAndLabels(scores, labels, output_base_path + SCORE_FILE_SUFFIX);
	WriteUtils::writeTime(total_time, scores.size(), output_base_path + TIME_FILE_SUFFIX);
}

Submatrix::Submatrix(int row_idx, int col_idx, double value) {
    submatrix_sum = value;
    submatrix_rows_count = 1;
    submatrix_cols_count = 1;

    rows_sum[row_idx] = value;
    cols_sum[col_idx] = value;
}

double Submatrix::getSubmatrixSum() {
    return submatrix_sum;
}

void Submatrix::updateSubmatrixSum(double value) {
    submatrix_sum += value;
}

int Submatrix::getSubmatrixRowsCount() {
    return submatrix_rows_count;
}

int Submatrix::getSubmatrixColsCount() {
    return submatrix_cols_count;
}

void Submatrix::addSubmatrixRow(int row_idx, double value, vector<vector<double>>& mat) {
    submatrix_rows_count++;
    rows_sum[row_idx] = value;
    for (auto& it: cols_sum) {
        cols_sum[it.first] += mat[row_idx][it.first];
    }
}

void Submatrix::addSubmatrixCol(int col_idx, double value, vector<vector<double>>& mat) {
    submatrix_cols_count++;
    cols_sum[col_idx] = value;
    for (auto& it: rows_sum) {
        rows_sum[it.first] += mat[it.first][col_idx];
    }
}

void Submatrix::delSubmatrixRow(int row_idx, vector<vector<double>>& mat) {
    submatrix_rows_count--;
    rows_sum.erase(row_idx);
    for (auto& it: cols_sum) {
        cols_sum[it.first] -= mat[row_idx][it.first];
    }
}

void Submatrix::delSubmatrixCol(int col_idx, vector<vector<double>>& mat) {
    submatrix_cols_count--;
    cols_sum.erase(col_idx);
    for (auto& it: rows_sum) {
        rows_sum[it.first] -= mat[it.first][col_idx];
    }
}

double Submatrix::getSubmatrixRowSum(int row_idx) {
    return rows_sum[row_idx];
}

double Submatrix::getSubmatrixColSum(int col_idx) {
    return cols_sum[col_idx];
}

void Submatrix::updateSubmatrixRowSum(int row_idx, double value) {
    rows_sum[row_idx] += value;
}

void Submatrix::updateSubmatrixColSum(int col_idx, double value) {
    cols_sum[col_idx] += value;
}

double Submatrix::getDensity() {
    return submatrix_sum/sqrt(submatrix_rows_count * submatrix_cols_count);
}

bool Submatrix::checkAndAdd(int row_idx, int col_idx, vector<vector<double>>& mat) {
    int cur_rows = submatrix_rows_count;
    int cur_cols = submatrix_cols_count;
    double cur_submatrix_row_sum = 0.0;
    double cur_submatrix_col_sum = 0.0;
    double cur_submatrix_sum = 0.0;

    bool row_flag = !(rows_sum.find(row_idx) == rows_sum.end());
    bool col_flag = !(cols_sum.find(col_idx) == cols_sum.end());

    if (row_flag && col_flag) {
        submatrix_sum = submatrix_sum + 1.0;
        rows_sum[row_idx] += 1.0;
        cols_sum[col_idx] += 1.0;
        return false;
    }

    if (!row_flag) {
        for (auto& it: cols_sum) {
            cur_submatrix_row_sum += mat[row_idx][it.first];
        }
        cur_rows++;
    }

    if (!col_flag) {
        for (auto& it: rows_sum) {
            cur_submatrix_col_sum += mat[it.first][col_idx];
        }
        cur_cols++;
    }

    if (!row_flag && !col_flag) {
        cur_submatrix_sum = submatrix_sum + cur_submatrix_row_sum + cur_submatrix_col_sum + mat[row_idx][col_idx];   
    } else {
        cur_submatrix_sum = submatrix_sum + cur_submatrix_row_sum + cur_submatrix_col_sum;
    }

    if (getDensity() < cur_submatrix_sum/sqrt(cur_rows * cur_cols)) {
        if (!row_flag & !col_flag) {
            addSubmatrixRow(row_idx, cur_submatrix_row_sum, mat);
            addSubmatrixCol(col_idx, cur_submatrix_col_sum + mat[row_idx][col_idx], mat);
        }
        else if (!row_flag) {
            addSubmatrixRow(row_idx, cur_submatrix_row_sum, mat);
        }
        else if (!col_flag) {
            addSubmatrixCol(col_idx, cur_submatrix_col_sum, mat);
        }
        submatrix_sum = cur_submatrix_sum;
        return true;
    }
    return false;
}

bool Submatrix::checkAndDel(vector<vector<double>>& mat) {
    pair<int, double> min_row = {-1, numeric_limits<double>::max()};
    if (submatrix_rows_count > 1) {
        for (auto& it: rows_sum) {
            if (it.second < min_row.second) {
                min_row = {it.first, it.second};
            }
        }
    }

    pair<int, double> min_col = {-1, numeric_limits<double>::max()};
    if (submatrix_cols_count > 1) {
        for (auto& it: cols_sum) {
            if (it.second < min_col.second) {
                min_col = {it.first, it.second};
            }
        }
    }
    double row_del_density =  0.0;
    if (min_row.first != -1) {
        row_del_density = (submatrix_sum - min_row.second)/sqrt((submatrix_rows_count-1)*submatrix_cols_count);
    }
    double col_del_density = 0.0;
    if (min_col.first != -1) {
        col_del_density = (submatrix_sum - min_col.second)/sqrt(submatrix_rows_count*(submatrix_cols_count-1));
    }

    double cur_density = getDensity();

    if (cur_density < row_del_density && col_del_density < row_del_density) {
        delSubmatrixRow(min_row.first, mat);
        submatrix_sum = submatrix_sum - min_row.second;
        return true;
    } else if (cur_density < col_del_density && row_del_density < col_del_density) {
        delSubmatrixCol(min_col.first, mat);
        submatrix_sum = submatrix_sum - min_col.second;
        return true;
    }
    return false;
}

void Submatrix::decay(double decay_factor) {
    submatrix_sum = submatrix_sum * decay_factor;
    for (auto& it: rows_sum) {
        rows_sum[it.first] = rows_sum[it.first] * decay_factor;
    }
    for (auto& it: cols_sum) {
        cols_sum[it.first] = cols_sum[it.first] * decay_factor;
    }
}

double Submatrix::getLikelihoodScore(int row_idx, int col_idx, vector<vector<double>>& mat) {
    double score = 0.0;
    double ctr = 0.0;
    for (auto& it: rows_sum) {
        score = score + mat[it.first][col_idx];
        ctr = ctr + 1.0;
    }
    for (auto& it: cols_sum) {
        score = score + mat[row_idx][it.first];
        ctr = ctr + 1.0;
    }

    bool row_flag = !(rows_sum.find(row_idx) == rows_sum.end());
    bool col_flag = !(cols_sum.find(col_idx) == cols_sum.end());
    if (row_flag && col_flag) {
        score = score - mat[row_idx][col_idx];
        ctr = ctr - 1.0;
    }
    return score/ctr;
}

vector<int> Submatrix::getRows() {
    vector<int> rows;
    for (auto& it: rows_sum) {
        rows.push_back(it.first);
    }
    return rows;
}

vector<int> Submatrix::getCols() {
    vector<int> cols;
    for (auto& it: cols_sum) {
        cols.push_back(it.first);
    }
    return cols;
}