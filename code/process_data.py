import pandas as pd
import numpy as np
import sys
import os

from numpy import savetxt


def process_dataset(base_path, dataset_name, time_param, edge_threshold):
    record_labels = generate_record_labels(base_path, dataset_name)
    labels = generate_final_labels(edge_threshold, record_labels, time_param)
    write_to_file(base_path, dataset_name, edge_threshold, labels, time_param)


def write_to_file(base_path, dataset_name, edge_threshold, labels, time_param):
    file_path = os.path.join(base_path, dataset_name, f"Label_{str(time_param)}_{str(edge_threshold)}.csv")
    savetxt(file_path, labels, delimiter='\n', fmt="%d")


def generate_final_labels(edge_threshold, record_labels, time_param):
    data = pd.DataFrame(np.array(record_labels))
    labels = []
    data[2] = (data[2] / time_param).astype(int)

    for i in pd.unique(data[2]):
        labels.append(sum(data[data[2] == i][3]))

    labels = np.array(labels)
    labels = labels >= edge_threshold
    labels = labels * 1
    return labels


def generate_record_labels(base_path, dataset_name):
    records = []
    data_path = os.path.join(base_path, dataset_name, "Data.csv")
    with open(data_path, "r") as f:
        for line in f:
            if len(line) <= 1:
                continue
            src, dst, time = line.split("\n")[0].split(",")
            records.append((int(src), int(dst), int(time)))

    labels = []
    labels_path = os.path.join(base_path, dataset_name, "Label.csv")
    with open(labels_path, "r") as f:
        for line in f:
            if len(line) <= 1:
                continue
            label = line.split("\n")[0]
            labels.append(int(label))

    assert len(records) == len(labels)

    record_labels = [(record[0], record[1], record[2], label) for record, label in zip(records, labels)]

    return record_labels


def main():
    dataset_name = str(sys.argv[1])
    time_param = int(sys.argv[2])
    edge_threshold = int(sys.argv[3])
    process_dataset("../data/", dataset_name, time_param, edge_threshold)


if __name__ == "__main__":
    main()
