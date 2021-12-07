# deeperGcnProject

Make suew pytorch, torch-geometric, and obg are installed

You may want to create an environment to run this program in

To run the project navigate into the Python folder and use this command:

'''python3 main.py --use_gpu --conv_encode_edge --num_layers 7 --dataset ogbg-molhiv --block res+ --gcn_aggr softmax --t 1.0 --learn_t --epochs 5 --dropout 0.2 --lr 0.0001'''

The dataset should begin downloading, it may take a few minutes since it is a large data set
