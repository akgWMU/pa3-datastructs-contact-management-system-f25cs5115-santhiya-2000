# Contact Management System Benchmark

This project implements a **Contact Management System** in C using five different data structures: Array, Linked List, Hash Map, Binary Search Tree (BST), and AVL Tree. It also includes benchmarking programs to evaluate the performance of these data structures and visualize the results using Python.

---

## File Structure

### Contact System Implementation

| File | Description |
|------|-------------|
| `array.c` | Contact management system implemented using a dynamic array. |
| `ll.c` | Contact management system implemented using a linked list. |
| `hashmap.c` | Contact management system implemented using a hash map with separate chaining. |
| `bst.c` | Contact management system implemented using a binary search tree. |
| `avl.c` | Contact management system implemented using an AVL tree (self-balancing BST). |
| `array` | Compiled executable for `array.c`. |
| `ll` | Compiled executable for `ll.c`. |
| `hashmap` | Compiled executable for `hashmap.c`. |
| `bst` | Compiled executable for `bst.c`. |
| `avl` | Compiled executable for `avl.c`. |

These programs provide the **core contact system** with menu-driven operations: Insert, Search, Update, Delete, and Display contacts.

---

### Benchmark Programs

| File | Description |
|------|-------------|
| `array_performance.c` | Benchmark program for the array-based contact system. Measures Insert, Search, Update, Delete performance and writes results to CSV. |
| `ll_performance.c` | Benchmark program for the linked list-based contact system. |
| `hashmap_performance.c` | Benchmark program for the hash map-based contact system. |
| `bst_performance.c` | Benchmark program for the BST-based contact system. |
| `avl_performance.c` | Benchmark program for the AVL tree-based contact system. |
| `array_performance` | Compiled executable for `array_performance.c`. |
| `ll_performance` | Compiled executable for `ll_performance.c`. |
| `hashmap_performance` | Compiled executable for `hashmap_performance.c`. |
| `bst_performance` | Compiled executable for `bst_performance.c`. |
| `avl_performance` | Compiled executable for `avl_performance.c`. |

These programs generate **performance CSV files** for different numbers of contacts, capturing execution time for all operations.

---

### CSV Files

| File | Description |
|------|-------------|
| `performance_array.csv` | Performance results of array implementation. |
| `performance_ll.csv` | Performance results of linked list implementation. |
| `performance_hashmap.csv` | Performance results of hash map implementation. |
| `performance_bst.csv` | Performance results of BST implementation. |
| `performance_avl.csv` | Performance results of AVL tree implementation. |

---

### Python Visualization

| File | Description |
|------|-------------|
| `graph.py` | Generates individual graphs from CSV files for each data structure. |
| `performance_graph.py` | Generates comparison graphs for all data structures and saves them as PNG images. |

---

## How to Run

### Compile C Programs

```bash
gcc array.c -o array
gcc ll.c -o ll
gcc hashmap.c -o hashmap
gcc bst.c -o bst
gcc avl.c -o avl

gcc array_performance.c -o array_performance
gcc ll_performance.c -o ll_performance
gcc hashmap_performance.c -o hashmap_performance
gcc bst_performance.c -o bst_performance
gcc avl_performance.c -o avl_performance


### Run C Programs
./array          # Run array-based contact system
./ll             # Run linked list-based contact system
./hashmap        # Run hash map-based contact system
./bst            # Run BST-based contact system
./avl            # Run AVL-based contact system

### Run Programs with Performance
./array_performance
./ll_performance
./hashmap_performance
./bst_performance
./avl_performance
 These will generate the csv files


### Run Python Programs
python3 graph.py
python3 performance_graph.py

This will generate a png DataStructures_Benchmark.png with the graph for all the operations for each datastructures.


Project Objectives

Implement a contact management system using multiple data structures.

Evaluate the performance of each data structure in Insert, Search, Update, and Delete operations.

Benchmark the performance with different numbers of contacts (e.g., 100, 500, 1000).

Visualize performance comparisons using graphs in Python.

Save results in CSV and PNG formats for analysis and reporting.

Requirements

C compiler (gcc)

Python 3

Python libraries:

pandas

matplotlib

Install required libraries with:

pip install pandas matplotlib



The Array (List) offers very simple and fast sequential access, with insertions at the end being O(1), but insertions or deletions in the middle and search operations take O(n). It uses O(n) space. The main advantage is simplicity and speed for sequential access, while the drawback is expensive middle insertions/deletions and slow searches.

The Singly Linked List allows O(1) insertion at the head (and O(n) at the tail if there is no tail pointer), but search, update, and delete operations all take O(n). Memory usage is O(n) due to extra pointers in each node. Its advantages include easy insertion and deletion, but it suffers from slow searches and slightly higher memory overhead.

The Hash Map provides O(1) average time complexity for insert, search, update, and delete, though worst-case operations can degrade to O(n). Space complexity is O(n + m), where m is the hash table size. It is very fast on average for lookups but requires good hash functions, can be unordered, and resizing can be costly.

The Binary Search Tree (Unbalanced) has O(h) time for insert, search, update, and delete, where h is the tree height; in the worst case, this can degrade to O(n). Space usage is O(n). Its main benefit is maintaining data in sorted order, but performance can degrade if the tree becomes unbalanced.

The Balanced BST (AVL Tree) guarantees O(log n) time for insert, search, update, and delete operations. Space complexity is O(n), with some extra memory to store balance information. It ensures balanced performance and sorted data but is more complex to implement and requires rotations, which add some overhead.
