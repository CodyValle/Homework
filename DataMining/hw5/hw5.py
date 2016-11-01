"""
Cody Valle, Andrew Burke, Katreina Carpenter
Data Mining Homework 5 Program
"""

import copy
import math

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

"""
Calculates the least entropy of the passed in attribute values
"""
def calculate_least_entropy(table, indices):
    min_ent = 2
    for index in indices:
        ent = calc_enew(table, index, SURVIVED)
        if ent < min_ent:
            min_ent = ent
            ret = index
    return ret

"""Returns the class frequencies for each attribute value:
{att_val:[{class1: freq, class2: freq, ...}, total], ...}
"""
def attribute_frequencies(instances, att_index, class_index):
    # get unique list of attribute and class values
    att_vals = list(set(get_categories(instances, att_index)))
    class_vals = list(set(get_categories(instances, class_index)))
    # initialize the result
    result = {v: [{c: 0 for c in class_vals}, 0] for v in att_vals}
    # build up the frequencies
    for row in instances:
        label = row[class_index]
        att_val = row[att_index]
        result[att_val][0][label] += 1
        result[att_val][1] += 1
    return result

def calc_enew(instances, att_index, class_index):
    # get the length of the partition
    D = len(instances)
    # calculate the partition stats for att_index (see below)
    freqs = attribute_frequencies(instances, att_index, class_index)
    # find E_new from freqs (calc weighted avg)
    E_new = 0
    for att_val in freqs:
        D_j = float(freqs[att_val][1])
        probs = [(c/D_j) for (_, c) in freqs[att_val][0].items()]
        E_D_j = -sum([p*math.log(p,2) for p in probs])
        E_new += (D_j/D)*E_D_j
    return E_new

"""
Performs step 1 of the homework
"""
def step1(table):
    #table.sort(key=lambda x: x[SURVIVED])
    folds = partition_into_folds(copy.deepcopy(table), 10, SURVIVED)

    att_indices = [CLASS, AGE, SEX]
    index = calculate_least_entropy(table, att_indices)
    att_indices.pop(index)
    print att_indices
    

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
