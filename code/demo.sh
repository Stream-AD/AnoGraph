make clean
make

if [ $1 == "DARPA" ]; then

  echo "Preprocessing Graph Labels according to time_window and edge_threshold"
  python3 process_data.py DARPA 30 50

  echo "Running AnoGraph"
  ./main anograph DARPA 30 50 2 32

  echo "Running AnoGraph-K"
  ./main anograph_k DARPA 30 50 2 32 5

  echo "Running AnoEdge-G"
  # Algorithm => anoedge_g
  # Dataset => DARPA
  # Rows => 2
  # Buckets => 32
  # Decay factor => 0.9
  ./main anoedge_g DARPA 2 32 0.9

  echo "Running AnoEdge-L"
  # Algorithm => anoedge_g
  # Dataset => DARPA
  # Rows => 2
  # Buckets => 32
  # Decay factor => 0.9
  ./main anoedge_l DARPA 2 32 0.9

  echo "Installing python dependencies"
  pip3 install -r requirements.txt

  echo "Running python metrics"
  python3 metrics.py --dataset DARPA --time_window 30 --edge_threshold 50

fi

if [ $1 == "ISCX" ]; then

  echo "Preprocessing Graph Labels according to time_window and edge_threshold"
  python3 process_data.py ISCX 60 100

  echo "Running AnoGraph"
  ./main anograph ISCX 60 100 2 32

  echo "Running AnoGraph-K"
  ./main anograph_k ISCX 60 100 2 32 5

  echo "Running AnoEdge-G"
  ./main anoedge_g ISCX 2 32 0.9

  echo "Running AnoEdge-L"
  ./main anoedge_l ISCX 2 32 0.9

  python3 metrics.py --dataset ISCX --time_window 60 --edge_threshold 100

fi

if [ $1 == "IDS2018" ]; then

  echo "Preprocessing Graph Labels according to time_window and edge_threshold"
  python3 process_data.py IDS2018 60 100

  echo "Running AnoGraph"
  ./main anograph IDS2018 60 100 2 32

  echo "Running AnoGraph-K"
  ./main anograph_k IDS2018 60 100 2 32 5

  echo "Running AnoEdge-G"
  ./main anoedge_g IDS2018 2 32 0.9

  echo "Running AnoEdge-L"
  ./main anoedge_l IDS2018 2 32 0.9

  python3 metrics.py --dataset IDS2018 --time_window 60 --edge_threshold 100

fi

if [ $1 == "DDOS2019" ]; then

  echo "Preprocessing Graph Labels according to time_window and edge_threshold"
  python3 process_data.py DDOS2019 60 100

  echo "Running AnoGraph"
  ./main anograph DDOS2019 60 100 2 32

  echo "Running AnoGraph-K"
  ./main anograph_k DDOS2019 60 100 2 32 5

  echo "Running AnoEdge-G"
  ./main anoedge_g DDOS2019 2 32 0.9

  echo "Running AnoEdge-L"
  ./main anoedge_l DDOS2019 2 32 0.9

  python3 metrics.py --dataset DDOS2019 --time_window 60 --edge_threshold 100

fi

