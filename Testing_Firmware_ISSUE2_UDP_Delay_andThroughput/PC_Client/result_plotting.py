# Packages
import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os
import numpy as np

# Data folder structure

# Data
# -> 1m
#   -> 63
#     -> DEVICE.CSV
#     -> PC.csv
#   -> 127
#     -> DEVICE.CSV
#     -> PC.csv
# -> 5m

# PC and DEVICE file in each folder
csv_name_PC = "PC.csv"
csv_name_DEVICE = "DEVICE.csv"

# All data sets
col_name_PC = ["iteration","throughput","us_per_buffer"]
col_name_DEVICE = ["iteration","pack","totaltime","us_per_pack","packs_per_sec","bytes_per_sec",";"]

# Target data sets: Pick desired sets' name from all data set's name above
target_name_PC = ["iteration","us_per_buffer"]# 1 second / i packets
target_name_DEVICE = ["iteration","us_per_pack"]

# Plot graphs from Data folder
def folder_plotting(folder_path):
    for (dirpath, dirnames, filenames) in os.walk(folder_path):
        dirpath = dirpath.replace('\\','/')
        if not dirnames:
            figure_title = dirpath.split('/')[-2] + dirpath.split('/')[-1]# 1m63
            figure_name = figure_title + '.png'
            #print(dirpath, dirnames, filenames)
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
            plt.subplot(212)
            plt.plot(x2, y2)
            plt.savefig(figure_name)
            plt.close()

# Load each csv file
def data_loader(csv_file, col_names, target_names):
    df = pd.read_csv(csv_file, delimiter=";", names= col_names, skiprows=[0,1,3])
    return pd.to_numeric(df[target_names[0]][2:], downcast='float'), pd.to_numeric(df[target_names[1]][2:], downcast='float')

# Main function
def main():
    parser = argparse.ArgumentParser ()
    parser.add_argument('--folder', type = str, help = 'The location of the data folder.', default = './Data')
    args = parser.parse_args ()
    folder_plotting(args.folder)

if __name__ == "__main__":
    main()
