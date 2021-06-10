#ifndef anographk_hpp
#define anographk_hpp

#include <vector>
using namespace std;

class AnographK {
    public:
    	AnographK(string algorithm, string dataset_name, int time_window, int edge_threshold, int rows, int buckets, int topK);
    	void run();
    	static double getAnographKDensity(vector<vector<double>>& mat, int K);

	private:
		string algorithm, dataset_name;
		int rows, buckets, time_window, edge_threshold, topK;

		vector<pair<vector<int>, vector<int>>> graphs;
		vector<int> labels;

		vector<double> getScores();

		static double getSubgraphDensity(vector<vector<double>>& mat, int src, int dst);
};

#endif