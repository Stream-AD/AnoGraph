#ifndef anograph_hpp
#define anograph_hpp

#include <vector>
using namespace std;

class Anograph {
    public:
    	Anograph(string algorithm, string dataset_name, int time_window, int edge_threshold, int rows, int buckets);
    	void run();
    	static double getAnographDensity(vector<vector<double>>& mat);

	private:
		string algorithm, dataset_name;
		int rows, buckets, time_window, edge_threshold;

		vector<pair<vector<int>, vector<int>>> graphs;
		vector<int> labels;

		vector<double> getScores();

		static pair<int, double> pickMinRow(vector<vector<double>>& mat, bool row_flag[], bool col_flag[]);
		static pair<int, double> pickMinCol(vector<vector<double>>& mat, bool row_flag[], bool col_flag[]);
		static double getMatrixDensity(vector<vector<double>>& mat, bool row_flag[], bool col_flag[]);
};

#endif