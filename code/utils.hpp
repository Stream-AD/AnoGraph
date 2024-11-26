#ifndef utils_hpp
#define utils_hpp

#include <vector>
using namespace std;

const string DATA_BASE_PATH = "../data/";
const string RESULT_BASE_PATH = "../results/";
const string SCORE_FILE_SUFFIX = "_score.csv";
const string TIME_FILE_SUFFIX = "_time.csv";

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

class WriteUtils {
    public:
		static void writeScoresAndLabels(vector<double> scores, vector<int> labels, string output_file);
		static void writeTime(double total_time, size_t num_records, string output_file);
};

class ReadUtils {
	public:
		static void loadEdgeData(vector<int>& src, vector<int>& dst, vector<int>& times, string dataset_name);
		static void loadEdgeLabelData(vector<int>& labels, string dataset_name);
		static void loadGraphData(vector<pair<vector<int>, vector<int>>>& graphs, vector<int>& labels, string dataset_name, int time_window, int edge_threshold);
};

#endif