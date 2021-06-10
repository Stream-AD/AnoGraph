import pandas as pd
import numpy as np
import sys

from numpy import savetxt

def process_dataset(base_path, dataset_name, time_param, edge_thershold):
	records = []
	with open(base_path + dataset_name + "/Data.csv", "r") as f:
		for line in f:
			if len(line) <= 1:
				continue
			src, dst, time = line.split("\n")[0].split(",")
			records.append((int(src), int(dst), int(time)))

	labels = []
	with open(base_path + dataset_name + "/Label.csv", "r") as f:
		for line in f:
			if len(line) <= 1:
				continue
			label = line.split("\n")[0]
			labels.append(int(label))

	assert len(records) == len(labels)

	record_labels = [(record[0], record[1], record[2], label) for record, label in zip(records, labels)]

	write_format = str(time_param) + "_" + str(edge_thershold)

	data = pd.DataFrame(np.array(record_labels))
	
	labels = []
	data[2] = (data[2]/time_param).astype(int)
	for i in pd.unique(data[2]):
	    labels.append(sum(data[data[2]==i][3]))

	labels = np.array(labels)
	labels = labels >= edge_thershold
	labels = labels * 1
	savetxt(base_path + dataset_name + "/Label_" + write_format + ".csv", labels, delimiter='\n',fmt="%d")

# print ("Number of arguments:", len(sys.argv), "arguments.")
# print ("Argument List:", str(sys.argv))
process_dataset("../data/", str(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]))
