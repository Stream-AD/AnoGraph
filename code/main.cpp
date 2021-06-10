#include <iostream>

#include "anoedgeglobal.hpp"
#include "anoedgelocal.hpp"
#include "anograph.hpp"
#include "anographk.hpp"
#include "utils.hpp"

void run_anoedge_global(int argc, char** argv) {
	if (argc != 6) {
		cout << "Expected five arguments but got " + to_string(argc) << endl;
		return;
	}

    string algorithm = argv[1];
	string dataset_name = argv[2];
	int rows = stoi(argv[3]);
	int buckets = stoi(argv[4]);
    double decay_factor = stof(argv[5]);

	AnoedgeGlobal anoedge_global(algorithm, dataset_name, rows, buckets, decay_factor);
	anoedge_global.run();
}

void run_anoedge_local(int argc, char** argv) {
	if (argc != 6) {
		cout << "Expected five arguments but got " + to_string(argc) << endl;
		return;
	}

    string algorithm = argv[1];
	string dataset_name = argv[2];
	int rows = stoi(argv[3]);
	int buckets = stoi(argv[4]);
    double decay_factor = stof(argv[5]);

	AnoedgeLocal anoedge_local(algorithm, dataset_name, rows, buckets, decay_factor);
	anoedge_local.run();
}

void run_anograph(int argc, char** argv) {
	if (argc != 7) {
		cout << "Expected seven arguments but got " + to_string(argc) << endl;
		return;
	}

    string algorithm = argv[1];
	string dataset_name = argv[2];
	int time_window = stoi(argv[3]);
	int edge_threshold = stoi(argv[4]);
	int rows = stoi(argv[5]);
	int buckets = stoi(argv[6]);

	Anograph anograph(algorithm, dataset_name, time_window, edge_threshold, rows, buckets);
	anograph.run();
}

void run_anographk(int argc, char** argv) {
	if (argc != 8) {
		cout << "Expected eight arguments but got " + to_string(argc) << endl;
		return;
	}

    string algorithm = argv[1];
	string dataset_name = argv[2];
	int time_window = stoi(argv[3]);
	int edge_threshold = stoi(argv[4]);
	int rows = stoi(argv[5]);
	int buckets = stoi(argv[6]);
	int K = stoi(argv[7]);

	AnographK anographk(algorithm, dataset_name, time_window, edge_threshold, rows, buckets, K);
	anographk.run();
}

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "Expected at least two arguments but got " + to_string(argc) << endl;
		return 0;
	}
	string algorithm = argv[1];
	if (algorithm == "anoedge_g") {
		run_anoedge_global(argc, argv);
	} else if (algorithm == "anoedge_l") {
		run_anoedge_local(argc, argv);
	} else if (algorithm == "anograph") {
		run_anograph(argc, argv);
	} else if (algorithm == "anograph_k") {
		run_anographk(argc, argv);
	} else {
		cout << "Algorithm " << algorithm << " doesn't match with any of the available algorithms (anoedge_g, anoedge_l, anograph, anograph_k)" << endl;
	}
	return 0;
}