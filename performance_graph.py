import pandas as pd
import matplotlib.pyplot as plt

# CSV files and corresponding data structures
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

# Operations
operations = ["Insert", "Search", "Update", "Delete"]

# Create figure with 4 subplots
fig, axs = plt.subplots(2, 2, figsize=(14,10))
axs = axs.flatten()

# Color and line style map for clarity
styles = {
    "Array": ("blue", "o-"),
    "LinkedList": ("green", "s--"),
    "HashMap": ("red", "^-"),
    "BST": ("purple", "d-."),
    "AVL": ("orange", "x:")
}

for i, op in enumerate(operations):
    ax = axs[i]
    op_data = all_data[all_data['Operation'] == op]

    for ds in csv_files.keys():
        ds_data = op_data[op_data['DataStructure'] == ds]
        color, style = styles[ds]
        ax.plot(ds_data['Contacts'], ds_data['Time_ms'], style, color=color, label=ds)

    ax.set_title(f'{op} Operation')
    ax.set_xlabel('Number of Contacts')
    ax.set_ylabel('Time (ms)')
    ax.grid(True)
    ax.legend()

plt.suptitle('Benchmark Comparison of Data Structures', fontsize=16)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])

# Save the figure as PNG
plt.savefig('DataStructures_Benchmark.png', dpi=300)
plt.show()
