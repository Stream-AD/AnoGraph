#include <iostream>
#include <cassert>

#include "utils.hpp"

using namespace std;

void WriteUtils::writeScoresAndLabels(vector<double> scores, vector<int> labels, string output_file) {
	assert (scores.size() == labels.size());
	FILE* output_file_ptr = fopen(output_file.c_str(), "w");

    if (output_file_ptr == NULL) {
        cerr << "file path is : " << output_file << endl;
        cerr << "Can not open file to write" << endl;
        return;
    }

    for (size_t i = 0; i < scores.size(); i++) {
        fprintf(output_file_ptr, "%.4f %d\n", scores[i], labels[i]);
    }
    fclose(output_file_ptr);
}

void WriteUtils::writeTime(double total_time, size_t num_records, string output_file) {
	FILE* output_file_ptr = fopen(output_file.c_str(), "w");
	fprintf(output_file_ptr, "Average time taken: %.4fs\n", (1.0f*total_time)/num_records);
	fprintf(output_file_ptr, "Total time taken: %.4fs\n", 1.0f*total_time);
    fclose(output_file_ptr);
}

void ReadUtils::loadEdgeData(vector<int>& src, vector<int>& dst, vector<int>& times, string dataset_name) {
    string edges_file = DATA_BASE_PATH + dataset_name + "/Data.csv";
	FILE* edges_file_ptr = fopen(edges_file.c_str(), "r");
    if (edges_file_ptr == NULL) {
        cerr << "Could not read file " << edges_file << "\n";
        exit(0);
    }

    int s, d, t;
    while (fscanf(edges_file_ptr, "%d,%d,%d", &s, &d, &t) == 3) {
        src.push_back(s);
        dst.push_back(d);
        times.push_back(t);
    }
    return;
}

void ReadUtils::loadEdgeLabelData(vector<int>& labels, string dataset_name) {
    string labels_file = DATA_BASE_PATH + dataset_name + "/Label.csv";
    FILE* labels_file_ptr = fopen(labels_file.c_str(), "r");
    if (labels_file_ptr == NULL) {
        cerr << "Could not read file " << labels_file_ptr << "\n";
        exit(0);
    }
    int l;
    while (fscanf(labels_file_ptr, "%d", &l) == 1) {
        labels.push_back(l);
    }
    fclose(labels_file_ptr);
}

void ReadUtils::loadGraphData(vector<pair<vector<int>, vector<int>>>& graphs, vector<int>& labels, string dataset_name, int time_window, int edge_threshold) {
    string graphs_file = DATA_BASE_PATH + dataset_name + "/Data.csv";
    string labels_file = DATA_BASE_PATH + dataset_name + "/Label_" + to_string(time_window) + "_" + to_string(edge_threshold) + ".csv";

	FILE* labels_file_ptr = fopen(labels_file.c_str(), "r");
    if (labels_file_ptr == NULL) {
        cerr << "Could not read file " << labels_file << "\n";
        exit(0);
    }
    int l;
    while (fscanf(labels_file_ptr, "%d", &l) == 1) {
        labels.push_back(l);
    }
    fclose(labels_file_ptr);

    FILE* graphs_file_ptr = fopen(graphs_file.c_str(), "r");
    if (graphs_file_ptr == NULL) {
        cerr << "Could not read file " << graphs_file << "\n";
        exit(0);
    }
    int s, d, t;
    int cur_time = 0, record_ctr = 0;
    vector<int> cur_src, cur_dst;
    while (fscanf(graphs_file_ptr, "%d,%d,%d", &s, &d, &t) == 3) {
    	if (t/time_window != cur_time) {
    		graphs.push_back({cur_src, cur_dst});

    		cur_time = t/time_window;
    		cur_src.clear();
    		cur_dst.clear();
    	}
    	cur_src.push_back(s);
    	cur_dst.push_back(d);
		record_ctr++;
    }
    graphs.push_back({cur_src, cur_dst});
    fclose(graphs_file_ptr);

    assert (graphs.size() == labels.size());
    return;
}