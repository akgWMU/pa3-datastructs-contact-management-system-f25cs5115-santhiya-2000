import pandas as pd
import matplotlib.pyplot as plt

# List of CSV files and corresponding data structure names
csv_files = {
    "Array": "performance_array.csv",
    "LinkedList": "performance_ll.csv",
    "HashMap": "performance_hashmap.csv",
    "BST": "performance_bst.csv",
    "AVL": "performance_avl.csv"
}

# Read all CSVs into a single DataFrame
df_list = []
for ds, file in csv_files.items():
    df = pd.read_csv(file)
    df['DataStructure'] = ds
    df_list.append(df)

all_data = pd.concat(df_list, ignore_index=True)

# List of operations
operations = ["Insert", "Search", "Update", "Delete"]

# Plot graph for each operation
for op in operations:
    plt.figure(figsize=(8,6))
    op_data = all_data[all_data['Operation'] == op]

    # Plot each data structure
    for ds in csv_files.keys():
        ds_data = op_data[op_data['DataStructure'] == ds]
        plt.plot(ds_data['Contacts'], ds_data['Time_ms'], marker='o', label=ds)

    plt.title(f'{op} Operation Benchmark Comparison')
    plt.xlabel('Number of Contacts')
    plt.ylabel('Time (ms)')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()
