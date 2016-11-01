"""
Cody Valle, Andrew Burke, Katreina Carpenter
Data Mining Homework 5 Program
"""

import copy
import math
import random


""" Reads a csv file and returns a table (list of lists) """
def read_csv(filename):
    import csv
    the_file = open(filename, 'r')
    the_reader = csv.reader(the_file, dialect='excel')
    table = []
    for row in the_reader:
        if len(row) > 0:
            table.append(row)
    the_file.close()
    return table

"""
Counts occurences of an element in a column
"""
def count_occurences(table, index, value):
    count = 0
    for row in table:
        if row[index] == value:
            count += 1
    return count

""" Makes floats """
def get_column_as_floats(table, index):
    column = []
    for row in table:
        if row[index] != 'NA':
            column.append(float(row[index]))
    return column

"""
Gets a list of all different categorical values
"""
def get_categories(table, index):
    values = []
    for row in table:
        if row[index] not in values:
            values.append(row[index])
    return values

"""
Partitions the passed in table into k folds.
"""
def partition_into_folds(table, k, class_index):
    folds = [[] for _ in range(k)] # Create disjoint empty lists
    table.sort(key=lambda x: x[class_index]) # Sort by class_index
    cur_fold = 0
    for row in table:
        folds[cur_fold].append(row)
        cur_fold = (cur_fold + 1) % k

    return folds

"""Returns the class frequencies for each attribute value:
{att_val:[{class1: freq, class2: freq, ...}, total], ...}
"""
def attribute_frequencies(instances, att_index, class_index):
    # Get unique list of attribute and class values
    att_vals = get_categories(instances, att_index)
    class_vals = get_categories(instances, class_index)
    
    # Initialize the result
    result = {v: [{c: 0 for c in class_vals}, 0] for v in att_vals}
    
    # Build up the frequencies
    for row in instances:
        label = row[class_index]
        att_val = row[att_index]
        result[att_val][0][label] += 1
        result[att_val][1] += 1
    return result

def calc_enew(instances, att_index, class_index):
    # Get the length of the partition
    D = len(instances)
    
    # Calculate the partition stats for att_index (see below)
    freqs = attribute_frequencies(instances, att_index, class_index)
    
    # find E_new from freqs (calc weighted avg)
    E_new = 0
    for att_val in freqs:
        D_j = float(freqs[att_val][1])
        probs = [(c/D_j) for (_, c) in freqs[att_val][0].items()]
        E_D_j = -sum([0 if p == 0.0 else p*math.log(p,2) for p in probs])
        E_new += (D_j/D)*E_D_j
    return E_new

"""
Calculates the least entropy of the passed in attribute values
"""
def calculate_least_entropy(table, indices, class_index):
    min_ent = 2
    for index in indices:
        ent = calc_enew(table, index, class_index)
        if ent < min_ent:
            min_ent = ent
            ret = index
    return ret

"""
Creates a Decision Tree based on the passed in attributes
"""
def decision_tree(table, attrs, class_index):
    """ Check three conditions """
    # No rows
    if len(table) == 0:
        return (None,None)

    # No attributes
    if len(attrs) == 0:
        l = float(len(table))
        values = get_categories(table, class_index)
        options = []
        for value in values:
            options.append((value, count_occurences(table, class_index, value) / l))

        return (None, options)

    # All labels are the same
    labels = get_categories(table, class_index)
    num_max_label =  max([count_occurences(table, class_index, label) for label in labels])
    if num_max_label == len(table):
        return (None, [(table[0][class_index], 1.0)])
    
    # Calculate the smallest entropy
    index = calculate_least_entropy(table, attrs, class_index)

    # Partition on that index
    values = get_categories(table, index)
    partitions = {value : [] for value in values}
    for row in table:
        partitions[row[index]].append(row)

    # Create a decision tree on each partition
    new_attrs = copy.deepcopy(attrs)
    new_attrs.pop(attrs.index(index))
    sub_trees = {}
    for value in values:
        sub_trees[value] = decision_tree(partitions[value], new_attrs, class_index)

    return (index, sub_trees)

"""
Uses the decision tree to determine the class label
"""
def decide(tree, x):
    if tree[0] == None:
        prob = random.randint(0, 99) / 100.
        options = tree[1]
        for option in options:
            prob -= option[1]
            if prob <= 0:
                return option[0]
    return decide(tree[1][x[tree[0]]], x)

"""
Performs step 1 of the homework
"""
def step1(table):
    #folds = partition_into_folds(copy.deepcopy(table), 10, SURVIVED)

    #table.sort(key=lambda x: x[SURVIVED])
    #table = table[:712]
    tree = decision_tree(copy.deepcopy(table), [CLASS, AGE, SEX], SURVIVED)
    #print tree
    print decide(tree, ['crew','adult','male']) # 97.222% chance of 'no'
    
"""
Takes a table and removes every row that has an 'NA' present
"""
def clean_auto_data(table):
    ret = []
    for row in table:
        found = False
        for value in row:
            if value == 'NA':
                found = True # There is an element in this row with 'NA'
                break
        if not found: # If there are no elements with 'NA'
            ret.append(row) # Add the row to the return table

    return ret

"""
The main function for this program
"""
def main():
    # Some index references to use globally
    global MPG
    MPG = 0
    global CYLINDERS
    CYLINDERS = 1
    global DISPLACEMENT
    DISPLACEMENT = 2
    global HORSEPOWER
    HORSEPOWER = 3
    global WEIGHT
    WEIGHT = 4
    global ACCELERATION
    ACCELERATION = 5
    global MODEL_YEAR
    MODEL_YEAR = 6
    global ORIGIN
    ORIGIN = 7
    global CAR_NAME
    CAR_NAME = 8
    global MSRP
    MSRP = 9
    global LABEL
    LABEL = 10

    global CLASS
    CLASS = 0
    global AGE
    AGE = 1
    global SEX
    SEX = 2
    global SURVIVED
    SURVIVED = 3

    auto_table = read_csv('auto-data.txt') # Read in the automotive data
    auto_table = clean_auto_data(auto_table) # Cleans the table

    titanic_table = read_csv('titanic.txt')[1:]

    step1(copy.deepcopy(titanic_table))

if __name__ == '__main__':
    main()
