import pandas as pd
import matplotlib.pyplot as plt


csv_name_PC = "Task_2_PC.csv"
csv_name_device = "Task_2_device.csv"
col_name_PC = ["iteration","throughput","us_per_buffer"]
col_name_device = ["iteration","pack","totaltime","us_per_pack","packs_per_sec","bytes_per_sec"]
target_name_PC = ["iteration","us_per_buffer"]# 1 second / i packets
target_name_device = ["iteration","us_per_pack"]


def result_plotting(csv_file, col_names, target_names):
    df = pd.read_csv(csv_file, delimiter=";", names= col_names)
    print(df)

    plt.plot(df[target_names[0]][1:], df[target_names[1]][1:])
    plt.xlabel("Iteration")
    plt.ylabel("PacketDelay: "+ target_names[-1])
    title = csv_file.split("/")[-1].strip(".csv") + ".png"
    plt.savefig(title)

result_plotting(csv_name_PC, col_name_PC, target_name_PC)
result_plotting(csv_name_device, col_name_device, target_name_device)
