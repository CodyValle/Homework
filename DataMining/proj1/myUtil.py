from tabulate import tabulate
import numpy as np
import math
import random

""" Strips a table of all rows on an index that are None """
def strip(table, index):
    return [row for row in table if row[index]]

""" Cleans the table of all rows with at least one None """
def clean(table, indices = None):
    if indices == None:
        indices = list(range(len(table[0])))
    keep = []
    rem = []
    for row in table:
        if None in [row[i] for i in indices]:
            rem.append(row)
        else:
            keep.append(row)
    return keep, rem

""" Gets a list of all different categorical values """
def get_categories(table, index):
    return list(set([row[index] for row in table if row[index] != None]))

""" Counts occurences of an element in a column """
def count_occurences(table, index, value):
    return len([1 for row in table if row[index] == value])

""" Performs summary statistics. Min, max, average, mode, and median. """
def summary_statistics(table, atts, indices):
    # Calculates stats for the table on the index
    def stats(index):
        # Counts occurences of an attribute in the table
        def count(val):
            return len([True for x in table if x == val])
        
        rows = [x[index] for x in table]
        att_freqs = [(x, count(x)) for x in set(rows)]
        att_freqs.sort(key=lambda x:x[1])      
        mode = att_freqs[-1][0]
        
        table.sort(key=lambda x:x[index])
        med_index = -len(table) / 2 + len(table) - 1
        med = table[med_index][ZONAL]
        if len(table) % 2 == 1:
            med += table[med_index + 1][ZONAL]
            med /= 2.

        return [min(rows), max(rows), float(sum(rows)) / len(table), med, mode]
    
    # Prepare the table for printing
    tab_table = [[atts[i]] + stats(i) for i in indices]
    
    # Print summary statistics
    print 'Summary Statistics'
    print tabulate(tab_table, headers = ['Metric', 'Min', 'Max', 'Avg', 'Med', 'Mode'])

""" Creates a bootstrap set """
def bootstrap(table, number = None):
    return [random.choice(table) for _ in range(len(table) if not number else number)]

""" Partitions the passed in table into stratified k folds """
def stratify(table, k, class_index):
    table.sort(key=lambda x: x[class_index]) # Sort by class_index
    folds = [[] for _ in range(k)] # Create disjoint empty lists
    for i,row in enumerate(table):
        folds[i % k].append(row)
    return folds

""" Creates a testing set and training set from a list of folds, on the index of the test fold """
def test_and_train(folds, index):
    return ([f for j in range(len(folds)) for f in folds[j] if index != j], folds[index])

""" Returns the class frequencies for each attribute value:
{att_val:[{class1: freq, class2: freq, ...}, total], ...}
"""
def attribute_frequencies(instances, att_index, class_index, class_labels = None):
    # Get unique list of attribute and class values
    att_vals = get_categories(instances, att_index)
    if class_labels == None:
        class_vals = get_categories(instances, class_index)
    
    # Initialize the result
    result = {v: [{c: 0 for c in class_vals}, 0] for v in att_vals}
    
    # Build up the frequencies
    for row in instances:
        result[row[att_index]][0][row[class_index]] += 1
        result[row[att_index]][1] += 1
    return result

""" Calculates the E_new of the passed in instances """
def calc_enew(instances, att_index, class_index, class_labels = None):
    # Calculate the partition stats for att_index (see below)
    freqs = attribute_frequencies(instances, att_index, class_index)
    
    # find E_new from freqs (calc weighted avg)
    E_new = 0
    for att_val in freqs:
        D_j = float(freqs[att_val][1])
        E_new += D_j * -sum([0. if p == 0.0 else p * math.log(p,2)
                             for p in [(c/D_j) for (_, c) in freqs[att_val][0].items()]])
    return E_new / len(instances)

""" Calculates the least entropy of the passed in attribute values """
def calculate_least_entropy(table, indices, class_index):
    ret = [(calc_enew(table, i, class_index), i) for i in indices]
    return min(ret, key=lambda x:x[0])[1]

""" Determines the best split points for the dataset using dummy threads """
def determine_split_points(table, index, class_index, num_threads = 16):
    """ Does work in a thread for the determine_split_points_threadpool function """
    def split_points_worker((start, stop)):
        return [(calc_enew(table[:i], index, class_index, labels) * len(table[:i])
                 +calc_enew(table[i:], index, class_index, labels) * len(table[i:]),
                    i) for i in range(max(1, start), stop)]

    from multiprocessing.dummy import Pool as ThreadPool
    import pickle

    # Sort on index and get all possible labels
    table.sort(key=lambda x:x[index])
    labels = get_categories(table, class_index)

    # Set up arguments
    l = float(len(table))
    args = [(int(l/num_threads * i + .5), int(l/num_threads * (i+1) + .5)) for i in range(num_threads)]

    # Get the results
    pool = ThreadPool(num_threads)
    results = [r for rs in pool.map(split_points_worker, args) for r in rs]

    # Join all the threads
    pool.close()
    pool.join()

    # Sort the results
    results.sort(key=lambda x:x[1])

    # Save to a file
    with open(str(index) + '->' + str(class_index) + '.pickle', 'wb') as handle:
        pickle.dump(results, handle)
