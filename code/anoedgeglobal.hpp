#ifndef anoedgeglobal_hpp
#define anoedgeglobal_hpp

#include <vector>
using namespace std;

class AnoedgeGlobal {
    public:
    	AnoedgeGlobal(string algorithm, string dataset_name, int rows, int buckets, double decay_factor);
    	void run();
    	static double getAnoedgeglobalDensity(vector<vector<double>>& mat, int src, int dst);

	private:
		string algorithm, dataset_name;
		int rows, buckets;
		double decay_factor;
		vector<int> src, dst, times, labels;

		vector<double> getScores();
};

#endif