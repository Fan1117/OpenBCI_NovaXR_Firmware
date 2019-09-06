import pandas as pd
cols = ['iteration','throughput','buffer']
df = pd.read_csv('Task_2_PC.csv', delimiter=";", names = cols)
print(df['throughput'][1:])
import matplotlib.pyplot as plt
plt.plot(df['iteration'], df['throughput'])
plt.show()

csv_PC = "../data/Task_2_PC"
csv_device = "../data/Task_2_device"
colname_PC = ["iteration","throughput","buffer"]
colname_device = ["iteration","pack","totaltime","us/packs","packs/sec","bytes/sec"]


def result_plotting(csv_file, col_names):
    df = pd.read_csv(csv_file, delimiter=";", names= col_names)
