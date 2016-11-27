"""
Cody Valle, Andrew Burke, Katreina Carpenter
Data Mining Homework 6 Program
"""

import copy
import math
import random
import operator

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
Takes an instance and returns a label based on the MPG value.
"""
def categorize_mpg(table):
    for row in table:
        if float(row[MPG]) <= 13:
            row[MPG] = 1
        elif float(row[MPG]) <= 15:
            row[MPG] = 2
        elif float(row[MPG]) <= 17:
            row[MPG] = 3
        elif float(row[MPG]) <= 20:
            row[MPG] = 4
        elif float(row[MPG]) <= 24:
            row[MPG] = 5
        elif float(row[MPG]) <= 27:
            row[MPG] = 6
        elif float(row[MPG]) <= 31:
            row[MPG] = 7
        elif float(row[MPG]) <= 37:
            row[MPG] = 8
        elif float(row[MPG]) <= 45:
            row[MPG] = 9
        else:
            row[MPG] = 10

"""
Converts the weight column to categories
"""
def discretize_weight(table):
    for row in table:
        if float(row[WEIGHT]) <= 1999:
            row[WEIGHT] = '1'
        elif float(row[WEIGHT]) <= 2499:
            row[WEIGHT] = '2'
        elif float(row[WEIGHT]) <= 2999:
            row[WEIGHT] = '3'
        elif float(row[WEIGHT]) <= 3499:
            row[WEIGHT] = '4'
        else:
            row[WEIGHT] = '5'


""" Gets a list of all different categorical values """
def get_categories(table, index):
    values = []
    for row in table:
        if row[index] != None and row[index] not in values:
            values.append(row[index])
    return values

""" Counts occurences of an element in a column """
def count_occurences(table, index, value):
    count = 0
    for row in table:
        if row[index] == value:
            count += 1
    return count

""" Creates a bootstrap set """
def bootstrap(table, number = None):
    if number == None:
        number = len(table)
    ret = []
    for _ in range(number):
        ret.append(random.choice(table))
    return ret

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
        label = row[class_index]
        att_val = row[att_index]
        result[att_val][0][label] += 1
        result[att_val][1] += 1
    return result

def calc_enew(instances, att_index, class_index, class_labels = None):
    # Calculate the partition stats for att_index (see below)
    freqs = attribute_frequencies(instances, att_index, class_index)
    
    # find E_new from freqs (calc weighted avg)
    E_new = 0
    for att_val in freqs:
        D_j = float(freqs[att_val][1])
        probs = [(c/D_j) for (_, c) in freqs[att_val][0].items()]
        E_D_j = -sum([0. if p == 0.0 else p * math.log(p,2) for p in probs])
        E_new += D_j * E_D_j
    return E_new / len(instances)

"""
Calculates the least entropy of the passed in attribute values
"""
def calculate_least_entropy(table, indices, class_index):
    min_ent = calc_enew(table, indices[0], class_index)
    ret = indices[0]
    for index in indices[1:]:
        ent = calc_enew(table, index, class_index)
        if ent < min_ent:
            min_ent = ent
            ret = index
    return ret

"""
Creates a Decision Tree based on the passed in attributes
"""
def decision_tree(table, attrs, class_index, F = None):
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

    """ Create node and/or subtrees """
    # Calculate the smallest entropy on random F attrs
    sub_attrs = copy.deepcopy(attrs)
    if F != None:
        while len(sub_attrs) > F:
            del sub_attrs[random.randint(0, len(sub_attrs) - 1)]
    index = calculate_least_entropy(table, sub_attrs, class_index)

    # Partition on that index
    values = get_categories(table, attrs.index(index))
    partitions = {value: [] for value in values}
    for row in table:
        partitions[row[attrs.index(index)]].append(row)

    # Create a decision tree on each partition
    new_attrs = copy.deepcopy(attrs)
    del new_attrs[attrs.index(index)]
    sub_trees = {}
    for value in values:
        sub_trees[value] = decision_tree(partitions[value], new_attrs, class_index, F)

    return (attrs.index(index), sub_trees)

"""
Uses a decision tree to determine the class label
"""
def decide(tree, x, class_labels):
    if tree[0] == None:
        prob = random.uniform(0, 1)
        options = tree[1]
        for option in options:
            prob -= option[1]
            if prob <= 0:
                return option[0] 
    if x[tree[0]] not in tree[1]:
        return random.choice(class_labels)
    return decide(tree[1][x[tree[0]]], x, class_labels)

""" Tests a tree and returns its accuracy """
def test_tree(tree, test, class_index, class_labels):
    total, correct = len(test), 0
    for row in test:
        if row[class_index] == decide(tree, row, class_labels):
            correct += 1
    return float(correct) / total     

""" Creates an ensemble of decision trees """
# table is a subset of the whole table (2/3)
# attrs are the attributes to use for the decision tree
# class_index is the index of the class to guess
# class_labels are all the possible labels
# M is the number of trees to keep, M < N
# N is the number of trees to make
# F is the number of attributes to randomly select
def random_forest(table, attrs, class_index, class_labels, M, N, F):
    trees = []
    for _ in range(N):
        # Bootstrap for this tree
        boot = bootstrap(table)
        # Create the tree
        tree = decision_tree(boot, attrs, class_index, F)
        # Check its accuracy using 20 percent of the passed in table
        test = bootstrap(table, int(len(table) * 0.2))
        accuracy = test_tree(tree, test, class_index, class_labels)
        # Add it to the list
        trees.append((tree, accuracy))

    # Sort the trees by accuracy
    trees.sort(key= lambda x:x[1])
    # Get the M best trees
    ret = []
    for i in range(-1, -M - 1, -1):
        ret.append(trees[i][0])
    return ret

""" Makes a decision using a random forest """
def decide_random_forest(forest, instance, class_labels):
    guesses = {}
    for tree in forest:
        guess = decide(tree, instance, class_labels)
        guesses[guess] = guesses.get(guess, 0) + 1
    
    return max(guesses.iteritems(), key=operator.itemgetter(1))[0]    

"""
Partitions the passed in table into k folds.
"""
def partition_into_folds(table, k, class_index):
    folds = [[] for _ in range(k)] # Create disjoint empty lists
    table.sort(key=lambda x: x[class_index]) # Sort by class_index
    for row in table:
        folds[random.choice(range(k))].append(row)

    return folds

"""
Creates a testing set and training set from a list of folds, on the index of the test fold.
"""
def create_test_and_train_from_folds(folds, index):
    training_set = []
    # Build a new training set excluding the current fold (index)
    for j in range(len(folds)):
        if index != j:
            training_set += folds[j]
    return (training_set, folds[index])

""" Converts an mpg labl to index """
def convert_mpg(label):
    return label - 1

""" Converts a titanic label to an index """
def convert_titanic(label):
        return 0 if label == 'yes' else 1

""" Creates a confusion matrix from a results table """
# results is a 2-dimensional matrix that will be placed in the confusion matrix
# labels are the labels that will appear as the headers for the confusion matrix
# convert is a function that will convert a class label to an index
# header is a string that will appear in the top corner of the confusion matrix
# Returns a 2-tuple: (confusion_matrix, total_accuracy)
def confusion_matrix(results, labels, convert, header = 'Results'):
    from tabulate import tabulate
    
    tabbed = []
    total_correct = 0
    total_predictions = 0.0
    for label in labels:
        row = [label] # First column is the actual
        raw_data = results[convert(label)] # List of predictions
        row += raw_data # N columns of prediction data
        predictions = float(sum(raw_data)) # Total number of preditions for this actual label
        row += [predictions] # N + 2 column is total number of times actual was predicted
        if predictions == 0:
            row += [0] # If there were no predictions, accuracy is zero
        else:
            row += [results[convert(label)][convert(label)] / predictions * 100] # Last column is accuracy

        total_correct += results[convert(label)][convert(label)] # Sum the diagonal
        total_predictions += predictions
        tabbed.append(row)

    conf_mat = tabulate(tabbed, headers=[header] + labels + ['Total', 'Recognition(%)']) # Headers is the top row of the confusion matrix
    return (conf_mat, total_correct / total_predictions)

""" Runs a performance test """
# table is a list of rows to create test and train sets with
# class_index is the index of the class label in the rows in the table
# class_labels is a list of all possible labels that can be found, present or not
# attrs is a list of atribute indices to use to make trees
# M is the number of trees to keep in the random forest
# N is the total number of trees to create
# F is the number of attributes to randomly select
# convert is a function that converts a class label to its respective index
# header is the string to put in the top corner of the confusion matrix
# Returns a 2-tuple of 2-tuples. ((forest_confusion_matrix, forest_accuracy), (tree_confusion_matrix, tree_accuracy))
def performance_test(table, class_index, class_labels, attrs, M, N, F, convert, header = 'Results'):
    # Create test and train sets
    folds = partition_into_folds(table, 3, class_index)
    train, test = create_test_and_train_from_folds(folds, 0)

    # Create the random forest and lone tree
    forest = random_forest(train, attrs, class_index, class_labels, M, N, F)
    tree = decision_tree(train, attrs, class_index, class_labels)

    # Create and fill in the results matrix
    matrix_forest = [[0 for _ in range(len(class_labels))] for _ in range(len(class_labels))]
    matrix_tree = [[0 for _ in range(len(class_labels))] for _ in range(len(class_labels))]
    
    for row in test:
        guess_forest = decide_random_forest(forest, row, class_labels)
        guess_tree = decide(tree, row, class_labels)

        matrix_forest[convert(row[class_index])][convert(guess_forest)] += 1
        matrix_tree[convert(row[class_index])][convert(guess_tree)] += 1

    # Get the accuracies
    forest_data = confusion_matrix(matrix_forest, class_labels, convert, header)
    tree_data = confusion_matrix(matrix_tree, class_labels, convert, header)

    return (forest_data, tree_data)

""" Performs Step 1 of the homework """
def step1(auto_table):
    labels = get_categories(auto_table, MPG)
    
    folds = partition_into_folds(copy.deepcopy(auto_table), 3, MPG)
    train, test = create_test_and_train_from_folds(folds, 0)
    
    M = 5
    N = 30
    F = 3
    forest = random_forest(train, [CYLINDERS, WEIGHT, MODEL_YEAR], MPG, labels, M, N, F)

    correct = 0
    for row in test:
        if row[MPG] == decide_random_forest(forest, row, labels):
            correct += 1

    print 'Accuracy of the random forest:', str(float(correct) / len(test) * 100) + '%.'
    print ''
    
""" Performs Step 2 of the homework """
def step2(auto_table, titanic_table):
    M = 7
    N = 20
    F = 2
    
    mpg_labels = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    titanic_labels = ['yes', 'no']

    auto_data = performance_test(auto_table,
                                 MPG, mpg_labels,
                                 [CYLINDERS, WEIGHT, MODEL_YEAR],
                                 M, N, F,
                                 convert_mpg,
                                 'MPG')

    # Print out the results
    print 'MPG Classifier Results:'
    print 'Random Forest Confusion Matrix'
    print auto_data[0][0] # Print forest confusion matrix
    print 'Lone Tree Confusion Matrix'
    print auto_data[1][0] # Print tree confusion matrix
    print 'Random Forest accuracy:', str(auto_data[0][1] * 100) + '%.'
    print 'Lone Tree accuracy:', str(auto_data[1][1] * 100) + '%.'
    print ''

    titanic_data = performance_test(titanic_table,
                                 SURVIVED, titanic_labels,
                                 [CLASS, AGE, SEX],
                                 M, N, F,
                                 convert_titanic,
                                 'Survived')

    # Print out the results
    print 'Titanic Survival Classifier Results:'
    print 'Random Forest Confusion Matrix'
    print titanic_data[0][0] # Print forest confusion matrix
    print 'Lone Tree Confusion Matrix'
    print titanic_data[1][0] # Print tree confusion matrix
    print 'Random Forest accuracy:', str(titanic_data[0][1] * 100) + '%.'
    print 'Lone Tree accuracy:', str(titanic_data[1][1] * 100) + '%.'
    print ''

""" Performs step 3 of the homework """
def step3(auto_table, titanic_table):
    mpg_labels = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    titanic_labels = ['yes', 'no']
    
    N_range = list(range(20, 1001, 100))
    M_range = list(range(3, 101, 18))
    F_range = list(range(2, 5))
    L_range = 5 # Number of times to run each case
    
    titanic_results = []
    auto_results = []

    for F in F_range:
        if F > len(mpg_labels):
            continue
        
        for M in M_range:
            for N in N_range:
                if N < M:
                    continue

                titanic_forest_accuracy = 0
                titanic_tree_accuracy = 0
                auto_forest_accuracy = 0
                auto_tree_accuracy = 0

                # Run each multiple times
                for _ in range(L_range):
                    # Get the data
                    titanic_data = performance_test(titanic_table,
                                                    SURVIVED, titanic_labels,
                                                    [CLASS, AGE, SEX],
                                                    M, N, F,
                                                    convert_titanic,
                                                    'Survived')

                    auto_data = performance_test(auto_table,
                                                 MPG, mpg_labels,
                                                 [CYLINDERS, WEIGHT, MODEL_YEAR],
                                                 M, N, F,
                                                 convert_mpg,
                                                 'MPG')

                    titanic_forest_accuracy += titanic_data[0][1]
                    titanic_tree_accuracy += titanic_data[1][1]
                    auto_forest_accuracy += auto_data[0][1]
                    auto_tree_accuracy += auto_data[1][1]

                titanic_forest_accuracy /= float(L_range)
                titanic_tree_accuracy /= float(L_range)
                auto_forest_accuracy /= float(L_range)
                auto_tree_accuracy /= float(L_range)
                
                # Store the results
                titanic_results.append((M, N, F, titanic_forest_accuracy, titanic_tree_accuracy))
                auto_results.append((M, N, F, auto_forest_accuracy, auto_tree_accuracy))

    """ Decide on the best values for M, N, and F """
    # Sort on tree accuracy, then forest accuracy
    titanic_results.sort(key=lambda x:x[4])
    titanic_results.sort(key=lambda x:x[3]) 
    auto_results.sort(key=lambda x:x[4]) 
    auto_results.sort(key=lambda x:x[3])

    # Get the best M, N, and F values
    M_titanic = titanic_results[-1][0]
    N_titanic = titanic_results[-1][1]
    F_titanic = titanic_results[-1][2]
    M_auto = auto_results[-1][0]
    N_auto = auto_results[-1][1]
    F_auto = auto_results[-1][2]

    # Get the data again
    titanic_data = performance_test(titanic_table,
                                    SURVIVED, titanic_labels,
                                    [CLASS, AGE, SEX],
                                    M_titanic, N_titanic, F_titanic,
                                    convert_titanic,
                                    'Survived')

    auto_data = performance_test(auto_table,
                                     MPG, mpg_labels,
                                     [CYLINDERS, WEIGHT, MODEL_YEAR],
                                     M_auto, N_auto, F_auto,
                                     convert_mpg,
                                     'MPG')

    # Print out the results
    print 'Best M, N, and F values for Auto Dataset'
    print '  ', 'M:', M_auto, 'N:', N_auto, 'F:', F_auto
    print auto_data[0][0]
    print 'Random Forest accuracy:', str(auto_data[0][1] * 100) + '%.'
    print 'Lone Tree accuracy:', str(auto_data[1][1] * 100) + '%.'
    print 'Best M, N, and F values for Titanic Dataset'
    print '  ', 'M:', M_titanic, 'N:', N_titanic, 'F:', F_titanic
    print titanic_data[0][0]
    print 'Random Forest accuracy:', str(titanic_data[0][1] * 100) + '%.'
    print 'Lone Tree accuracy:', str(titanic_data[1][1] * 100) + '%.'
    print ''

""" Performs Step 4 of the homework """
def step4(table):
    labels = get_categories(table, 9)
    
    N_range = list(range(20, 1001, 80))
    M_range = list(range(3, 101, 12))
    F_range = list(range(2, 4))
    L_range = 5
    
    results = []

    for F in F_range:
        if F > len(labels):
            continue
        
        for M in M_range:
            for N in N_range:
                if N < M:
                    continue

                forest_accuracy = 0
                tree_accuracy = 0

                # Run each multiple times
                for _ in range(L_range):
                    # Get the data
                    data = performance_test(table,
                                            9, labels,
                                            range(8),
                                            M, N, F,
                                            convert_wisconsin,
                                            'Tumor')
                    
                    forest_accuracy += data[0][1]
                    tree_accuracy += data[1][1]

                forest_accuracy /= float(L_range)
                tree_accuracy /= float(L_range)
                
                # Store the results
                results.append((M, N, F, forest_accuracy, tree_accuracy))

    """ Decide on the best values for M, N, and F """
    # Sort on tree accuracy, then forest accuracy
    results.sort(key=lambda x:x[4])
    results.sort(key=lambda x:x[3])

    # Get the best M, N, and F values
    M = results[-1][0]
    N = results[-1][1]
    F = results[-1][2]

    # Get the data again
    data = performance_test(table,
                            9, labels,
                            range(8),
                            M, N, F,
                            convert_wisconsin,
                            'Tumor')

    # Print out the results
    print 'Best M, N, and F values for Tumor Dataset'
    print '  ', 'M:', M, 'N:', N, 'F:', F
    print 'Tumor Classifier Results:'
    print 'Random Forest Confusion Matrix'
    print data[0][0] # Print forest confusion matrix
    print 'Lone Tree Confusion Matrix'
    print data[1][0] # Print tree confusion matrix
    print 'Random Forest accuracy:', str(data[0][1] * 100) + '%.'
    print 'Lone Tree accuracy:', str(data[1][1] * 100) + '%.'
    print ''

def main():
    auto_table = read_csv('auto-data.txt') # Read in the automotive data
    auto_table = clean_auto_data(auto_table) # Cleans the table
    categorize_mpg(auto_table)
    discretize_weight(auto_table)
    
    step1(auto_table)

    titanic_table = read_csv('titanic.txt')
    
    step2(auto_table, titanic_table)
    step3(auto_table, titanic_table)

    wisconsin_table = read_csv('wisconsin.dat') 
    step4(wisconsin_table)

""" To make this an executable """
if __name__ == '__main__':
    main()
