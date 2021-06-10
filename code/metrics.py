import pandas as pd
import argparse
from sklearn import metrics
parser = argparse.ArgumentParser()
parser.add_argument('--dataset', default='DARPA')
parser.add_argument('--time_window', type=int, default=30)
parser.add_argument("--edge_threshold", type=int, default=50)
args = parser.parse_args()


def print_anoedge_auc_time(base_path, dataset_name, algorithm):
	data = pd.read_csv(base_path + algorithm + "_" + dataset_name + "_score.csv", names=['score', 'label'], sep=" ")
	time_values = pd.read_csv(base_path + algorithm + "_" + dataset_name + "_time.csv", names=['avg', 'total'], sep=" ")

	fpr, tpr, _ = metrics.roc_curve(data.label, data.score)
	auc = metrics.roc_auc_score(data.label, data.score)

	print ("%s,%s" % (algorithm, dataset_name))
	print ("AUC: %.3f" % (auc))
	print ("Time: %s\n" % (time_values["total"].iloc[1]))

def print_anograph_auc_time(base_path, dataset_name, time_window, edge_threshold, algorithm):
	data = pd.read_csv(base_path + algorithm + "_" + dataset_name + "_" + str(time_window) + "_" + str(edge_threshold) + "_score.csv", names=['score', 'label'], sep=" ")
	time_values = pd.read_csv(base_path + algorithm + "_" + dataset_name + "_" + str(time_window) + "_" + str(edge_threshold) + "_time.csv", names=['avg', 'total'], sep=" ")

	fpr, tpr, _ = metrics.roc_curve(data.label, data.score)
	auc = metrics.roc_auc_score(data.label, data.score)

	print ("%s,%s" % (algorithm, dataset_name))
	print ("AUC: %.3f" % (auc))
	print ("Time: %s\n" % (time_values["total"].iloc[1]))


if __name__ == "__main__":
	if args.dataset == 'DARPA':
		print_anograph_auc_time("../results/", "DARPA", args.time_window, args.edge_threshold, "anograph")
		print_anograph_auc_time("../results/", "DARPA", args.time_window, args.edge_threshold, "anograph_k")

		print_anoedge_auc_time("../results/", "DARPA", "anoedge_g")
		print_anoedge_auc_time("../results/", "DARPA", "anoedge_l")

	if args.dataset == 'ISCX':
		print_anograph_auc_time("../results/", "ISCX", args.time_window, args.edge_threshold, "anograph")
		print_anograph_auc_time("../results/", "ISCX", args.time_window, args.edge_threshold, "anograph_k")

		print_anoedge_auc_time("../results/", "ISCX", "anoedge_g")
		print_anoedge_auc_time("../results/", "ISCX", "anoedge_l")

	if args.dataset == 'IDS2018':
		print_anograph_auc_time("../results/", "IDS2018", args.time_window, args.edge_threshold, "anograph")
		print_anograph_auc_time("../results/", "IDS2018", args.time_window, args.edge_threshold, "anograph_k")

		print_anoedge_auc_time("../results/", "IDS2018", "anoedge_g")
		print_anoedge_auc_time("../results/", "IDS2018", "anoedge_l")

	if args.dataset == 'DDOS2019':
		print_anograph_auc_time("../results/", "DDOS2019", args.time_window, args.edge_threshold, "anograph")
		print_anograph_auc_time("../results/", "DDOS2019", args.time_window, args.edge_threshold, "anograph_k")

		print_anoedge_auc_time("../results/", "DDOS2019", "anoedge_g")
		print_anoedge_auc_time("../results/", "DDOS2019", "anoedge_l")