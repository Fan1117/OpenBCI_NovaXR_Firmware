import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os
import numpy as np


csv_name_PC = "PC.csv"
csv_name_DEVICE = "DEVICE.csv"
col_name_PC = ["iteration","throughput","us_per_buffer"]
col_name_DEVICE = ["iteration","pack","totaltime","us_per_pack","packs_per_sec","bytes_per_sec"]
target_name_PC = ["iteration","us_per_buffer"]# 1 second / i packets
target_name_DEVICE = ["iteration","us_per_pack"]

def folder_plotting(folder_path):
    for (dirpath, dirnames, filenames) in os.walk(folder_path):
        dirpath = dirpath.replace('\\','/')
        if not dirnames:
            figure_title = dirpath.split('/')[-2] + dirpath.split('/')[-1]# 1m63
            figure_name = figure_title + '.png'
            for filename in filenames:
                path = dirpath + '/' + filename
                if filename == 'PC.csv':
                    x1, y1 = data_loader(path, col_name_PC, target_name_PC)

                elif filename == 'DEVICE.CSV':
                    x2, y2 = data_loader(path, col_name_DEVICE, target_name_DEVICE)
                else:
                    print("No csv files!!!")
            plt.subplot(211)
            plt.title(figure_title + " | Top: PC | Bottom: DEVICE")
            plt.plot(x1, y1)
            plt.xticks(np.arange(0,600,100))
            plt.yticks(np.arange(min(y1),max(y1),500))
            plt.subplot(212)
            plt.plot(x2, y2)
            plt.xticks(np.arange(0,600,100))
            plt.yticks(np.arange(min(y2),max(y2),50))
            #plt.yticks(np.arange(2000,6000,500))
            #plt.xticks(np.arange(min(x2), max(x2)+1, 1.0))
            #plt.yticks(np.arange(min(y2), max(y2)+1, 1.0))
            plt.tight_layout()
            plt.savefig(figure_name)
            plt.close()


def data_loader(csv_file, col_names, target_names):
    df = pd.read_csv(csv_file, delimiter=";", names= col_names)
    return pd.to_numeric(df[target_names[0]][1:], downcast='float'), pd.to_numeric(df[target_names[1]][1:], downcast='float')



# def result_plotting(csv_file, col_names, target_names):
#     df = pd.read_csv(csv_file, delimiter=";", names= col_names)
#     #plt.figure(figsize=(20,10))
#     plt.plot(df[target_names[0]][1:], df[target_names[1]][1:])
#     #plt.xlabel("Iteration")
#     #plt.ylabel("PacketDelay: "+ target_names[-1])
#     # title = csv_file.split("/")[-1].strip(".csv") + ".png"
#     # plt.savefig(title)
#     # plt.close()

def main():
    parser = argparse.ArgumentParser ()
    parser.add_argument('--folder', type = str, help = 'The location of the data folder.', default = './Data')
    args = parser.parse_args ()
    folder_plotting(args.folder)

if __name__ == "__main__":
    main()
