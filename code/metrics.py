import pandas as pd
import argparse
from sklearn import metrics

parser = argparse.ArgumentParser()
parser.add_argument('--dataset', default='DARPA')
parser.add_argument('--time_window', type=int, default=30)
parser.add_argument("--edge_threshold", type=int, default=50)
args = parser.parse_args()

results_path = "../results/"


def print_anoedge_auc_time(base_path, dataset_name, algorithm):
    file_name = base_path + algorithm + "_" + dataset_name
    print_auc_time(algorithm, dataset_name, file_name)


def print_anograph_auc_time(base_path, dataset_name, time_window, edge_threshold, algorithm):
    file_name = base_path + algorithm + "_" + dataset_name + "_" + str(time_window) + "_" + str(edge_threshold)
    print_auc_time(algorithm, dataset_name, file_name)


def print_auc_time(algorithm, dataset_name, file_name):
    data = pd.read_csv(file_name + "_score.csv", names=['score', 'label'], sep=" ")
    time_values = pd.read_csv(file_name + "_time.csv", names=['avg', 'total'], sep=" ")

    fpr, tpr, _ = metrics.roc_curve(data.label, data.score)
    auc = metrics.roc_auc_score(data.label, data.score)

    print("%s,%s" % (algorithm, dataset_name))
    print("AUC: %.3f" % auc)
    print("Time: %s\n" % (time_values["total"].iloc[1]))


def run_with_dataset(dataset_name):
    print_anograph_auc_time(results_path, dataset_name, args.time_window, args.edge_threshold, "anograph")
    print_anograph_auc_time(results_path, dataset_name, args.time_window, args.edge_threshold, "anograph_k")
    print_anoedge_auc_time(results_path, dataset_name, "anoedge_g")
    print_anoedge_auc_time(results_path, dataset_name, "anoedge_l")


if __name__ == "__main__":
    datasets = ["DARPA", "ISCX", "IDS2018", "DDOS2019"]
    if args.dataset in datasets:
        run_with_dataset(args.dataset)
    else:
        print(f"Could not detect dataset {args.dataset}")
