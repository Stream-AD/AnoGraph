#ifndef anoedgelocal_hpp
#define anoedgelocal_hpp

#include <vector>
#include <unordered_map>
using namespace std;

class Submatrix {
    public:
        Submatrix(int row_idx, int col_idx, double value);

        double getSubmatrixSum();
        void updateSubmatrixSum(double value);

        int getSubmatrixRowsCount();
        int getSubmatrixColsCount();

        void addSubmatrixRow(int row_idx, double value, vector<vector<double>>& mat);
        void addSubmatrixCol(int col_idx, double value, vector<vector<double>>& mat);
        void delSubmatrixRow(int row_idx, vector<vector<double>>& mat);
        void delSubmatrixCol(int col_idx, vector<vector<double>>& mat);

        double getSubmatrixRowSum(int row_idx);
        double getSubmatrixColSum(int col_idx);
        void updateSubmatrixRowSum(int row_idx, double value);
        void updateSubmatrixColSum(int col_idx, double value);

        double getDensity();
        bool checkAndAdd(int row_idx, int col_idx, vector<vector<double>>& mat);
        bool checkAndDel(vector<vector<double>>& mat);

        void decay(double decay_factor);

        double getLikelihoodScore(int row_idx, int col_idx, vector<vector<double>>& mat);

        vector<int> getRows();
        vector<int> getCols();

    private:
        unordered_map<int, double> rows_sum, cols_sum;
        double submatrix_sum;
        int submatrix_rows_count;
        int submatrix_cols_count;
};

class AnoedgeLocal {
    public:
    	AnoedgeLocal(string algorithm, string dataset_name, int rows, int buckets, double decay_factor);
    	void run();
    	static double getAnoedgelocalDensity(vector<vector<double>>& mat, int src, int dst);

	private:
		string algorithm, dataset_name;
		int rows, buckets;
		double decay_factor;
		vector<int> src, dst, times, labels;

		vector<vector<Submatrix>> densest_matrices;

		vector<double> getScores();
};

#endif