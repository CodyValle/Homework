"""
Andrew Burke, Katreina Carpenter, Cody Valle
HW3
Performs Naive Bayes algorithm on the 'auto-data.txt' dataset.
"""

import copy
import random
from decimal import Decimal

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
Builds a Naive Bayes classifier
"""
def build_naive_bayes_classifier(table, classes):
    ret = {}
    for c in classes:
        # List possible class label values
        label_values = get_categories(table, c)

        # Fill dictionary with rows partitioned on class label values
        nb_column = {}
        for value in label_values:
            nb_column[value] = []

        for row in table:
            nb_column[row[c]].append(row)

        # Create return object
        ret[c] = nb_column

    return copy.deepcopy(ret)

"""
Guesses a class label for the passed in instance based on the passed
in class index and attribute indices.
"""
def guess_label_categorical(classifier, instance, attrs, class_index):
    nb_table = classifier[class_index]
    total_rows = Decimal(sum([len(tb) for _,tb in nb_table.items()]))
    guess = (Decimal(-1),'')
    for class_label,rows in nb_table.items():
        probability = Decimal(1)
        denominator = Decimal(len(rows))
        for attr in attrs:
            numerator = Decimal(0)
            for row in rows:
                if row[attr] == instance[attr]:
                    numerator += 1
            probability *= numerator / denominator
        probability *= denominator / total_rows

        if probability > guess[0]:
            guess = (probability,class_label)
            
    return guess[1]
                    
"""
Tests the categorical Naive Bayes classifier from Step 1
old step 2: select random instrances from the dataset, predict
   their corresponding mpg ranking, and then show their actual ranking
old step 3: compute the (multi-class) predictive accuracy and error
   rate of the classifiers using separate training and testing sets
   approch 1: random subsampling with k=10
   approach 2: stratified k-fold cross validation with k_10
old step 4: create confusion matrices for each classifer based on
   the stratified 10-fold cross validation results
"""
def test1_of_naive_bayes_classifier(classifier, table):
    from tabulate import tabulate
    # Test using step2 approach from HW3
    print '==========================================='
    print 'TEST 1: Naive Bayes MPG Classifier'
    print '==========================================='

    total_tests = 5
    random_instances = [random.randint(0, len(table) - 1) for _ in range(total_tests)]   
    correct = 0
    for index in random_instances:
        predicted = guess_label_categorical(classifier, table[index], [WEIGHT,CYLINDERS,MODEL_YEAR], MPG)
        print '  predicted:', predicted, '  actual:', table[index][MPG]
        if predicted == table[index][MPG]:
            correct += 1
    accuracy = float(correct) / total_tests
    print ''
    print ' For k=5 Random Subsampling:'
    print ' accuracy:', accuracy, ', error_rate:', (1 - accuracy)
    print ''

    # Confusion matrix
    matrix = [[0] * 10 for _ in range(10)]
    for row in table:
        predicted = guess_label_categorical(classifier, row, [WEIGHT,CYLINDERS,MODEL_YEAR], MPG)
        matrix[int(row[MPG]) - 1][int(predicted) - 1] += 1

    tabbed_table = []
    for i in range(10):
        total = sum(matrix[i]) * 1.0
        tabbed_table.append([str(i + 1)] +
                            [matrix[i][j] for j in range(len(matrix[i]))] +
                            [total] +
                            [100. * matrix[i][i] / (total if total != 0 else 1)])
    print tabulate(tabbed_table, headers = ['MPG', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', 'Total', 'Recognition (%)'])
        
"""
Takes an instance and returns a label based on the MPG value.
"""
def convert_mpg_to_categorical(table):
    for row in table:
        if float(row[MPG]) <= 13:
            row[MPG] = '1'
        elif float(row[MPG]) <= 15:
            row[MPG] = '2'
        elif float(row[MPG]) <= 17:
            row[MPG] = '3'
        elif float(row[MPG]) <= 20:
            row[MPG] = '4'
        elif float(row[MPG]) <= 24:
            row[MPG] = '5'
        elif float(row[MPG]) <= 27:
            row[MPG] = '6'
        elif float(row[MPG]) <= 31:
            row[MPG] = '7'
        elif float(row[MPG]) <= 37:
            row[MPG] = '8'
        elif float(row[MPG]) <= 45:
            row[MPG] = '9'
        else:
            row[MPG] = '10'

"""
Converts the weight column to categories
"""
def convert_weight_to_categorical(table):
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

"""
Step 1: 
"""
def step1(table):
    # Converts the weight attribute to categorical
    convert_weight_to_categorical(table)
    # Converts the MPG attribute to categorical
    convert_mpg_to_categorical(table)
    # Builds a Naive Bayes classifier
    classifier = build_naive_bayes_classifier(table, [MPG])

    # Test the classifier
    test1_of_naive_bayes_classifier(classifier, table)

"""
Guesses a class label for the passed in instance based on the passed
in class index and attribute indices. Can handle Gaussian distributions.
"""
def guess_label_with_continuous(classifier, instance, cat_attrs, cont_attrs, class_index):
    import math
    nb_table = classifier[class_index]
    total_rows = Decimal(sum([len(tb) for _,tb in nb_table.items()]))
    guess = (Decimal(-1),'')

    # Gaussian function
    def gaussian_probability(x, mean, sdev):
        first, second = Decimal(0), Decimal(0)
        if sdev > 0:
            first = Decimal(1 / (math.sqrt(2 * math.pi) * sdev))
            second = Decimal(math.e ** (-((x - mean) ** 2) / (2 * (sdev ** 2))))
        return first * second 

    for class_label,rows in nb_table.items():
        probability = Decimal(1)
        denominator = Decimal(len(rows))
        for attr in cat_attrs:
            numerator = Decimal(0)
            for row in rows:
                if row[attr] == instance[attr]:
                    numerator += 1
            probability *= numerator / denominator
            
        for attr in cont_attrs:
            x = float(instance[attr['index']])
            mean = attr['mean']
            sdev = attr['sdev']
            probability *= gaussian_probability(x, mean, sdev)
            
        probability *= denominator / total_rows

        if probability > guess[0]:
            guess = (probability,class_label)
            
    return guess[1]
"""
Tests the categorical Naive Bayes classifier from Step 1
old step 2: select random instrances from the dataset, predict
   their corresponding mpg ranking, and then show their actual ranking
old step 3: compute the (multi-class) predictive accuracy and error
   rate of the classifiers using separate training and testing sets
   approch 1: random subsampling with k=10
   approach 2: stratified k-fold cross validation with k_10
old step 4: create confusion matrices for each classifer based on
   the stratified 10-fold cross validation results
"""
def test2_of_naive_bayes_classifier(classifier, table):
    from tabulate import tabulate
    import numpy
    # Test using step2 approach from HW3
    print '==========================================='
    print 'TEST 2: Naive Bayes MPG Classifier'
    print '==========================================='

    total_tests = 5
    random_instances = [random.randint(0, len(table) - 1) for _ in range(total_tests)]   
    correct = 0
    
    weights = get_column_as_floats(table, WEIGHT)
    d = {'index' : WEIGHT,
         'mean' : 1.0 * sum(weights) / len(weights),
         'sdev' : numpy.std(weights)}
    
    for index in random_instances:
        predicted = guess_label_with_continuous(classifier, table[index], [MODEL_YEAR,CYLINDERS], [d], MPG)
        
        print '  predicted:', predicted, '  actual:', table[index][MPG]
        if predicted == table[index][MPG]:
            correct += 1
            
    # Print statistics from step3 of HW3
    accuracy = float(correct) / total_tests
    print ''
    print ' For k=5 Random Subsampling:'
    print ' accuracy:', accuracy, ', error_rate:', (1 - accuracy)
    print ''

    # Confusion matrix from step4 of HW3
    matrix = [[0] * 10 for _ in range(10)]
    for row in table:
        predicted = guess_label_with_continuous(classifier, row, [MODEL_YEAR,CYLINDERS], [d], MPG)
        matrix[int(row[MPG]) - 1][int(predicted) - 1] += 1

    tabbed_table = []
    for i in range(10):
        total = sum(matrix[i]) * 1.0
        tabbed_table.append([str(i + 1)] +
                            [matrix[i][j] for j in range(len(matrix[i]))] +
                            [total] +
                            [100. * matrix[i][i] / (total if total != 0 else 1)])
    print tabulate(tabbed_table, headers = ['MPG', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', 'Total', 'Recognition (%)'])

"""
Step2:
"""
def step2(table):
    # Converts the MPG attribute to categorical
    convert_mpg_to_categorical(table)
    # Builds a Naive Bayes classifier
    classifier = build_naive_bayes_classifier(table, [MPG])

    # Test the classifier
    test2_of_naive_bayes_classifier(classifier, table)

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
Performs the Naive Bayes Classifier to the Titanic Dataset
"""
def naive_bayes_part(table):
    from tabulate import tabulate
    
    # Builds a Naive Bayes classifier
    nb_classifier = build_naive_bayes_classifier(table, [SURVIVED])

    # Test using step2 approach from HW3
    print '==========================================='
    print 'TEST 3: Naive Bayes Survival Classifier (Titanic)'
    print '==========================================='

    total_tests = 10
    random_instances = [random.randint(0, len(table) - 1) for _ in range(total_tests)]   
    correct = 0
    
    for index in random_instances:
        predicted = guess_label_categorical(nb_classifier, table[index], [CLASS,AGE,SEX], SURVIVED)
        
        print '  predicted:', predicted, '  actual:', table[index][SURVIVED]
        if predicted == table[index][SURVIVED]:
            correct += 1
            
    # Print statistics from step3 of HW3
    accuracy = float(correct) / total_tests
    print ''
    print ' For k=10 Random Subsampling:'
    print ' accuracy:', accuracy, ', error_rate:', (1 - accuracy)
    print ''

    # Confusion matrix is a dictionary of dictionaries
    matrix = {'yes' : {'yes' : 0, 'no' : 0},
              'no' : {'yes' : 0, 'no' : 0}}
    for row in table:
        predicted = guess_label_categorical(nb_classifier, row, [CLASS,AGE,SEX], SURVIVED)
        matrix[row[SURVIVED]][predicted] += 1
    
    tabbed_table = []
    for i in ['yes','no']:
        total = sum([matrix[i][j] for j in ['yes','no']]) * 1.0
        tabbed_table.append([i] +
                            [matrix[i][j] for j in ['yes','no']] +
                            [total] +
                            [100. * matrix[i][i] / (total if total != 0 else 1)])
    print tabulate(tabbed_table, headers = ['SURVIVED', 'yes', 'no', 'Total', 'Recognition (%)'])

    numerator = matrix['yes']['yes'] + matrix['no']['no']
    total = numerator + matrix['yes']['no'] + matrix['no']['yes']
    accuracy = float(numerator) / total

    print '   accuracy:', accuracy, ' error_rate:', (1 - accuracy)

"""
Partitions the passed in table into k folds.
"""
def partition_into_folds(table, k, class_index):
    folds = [[]] * k
    table.sort(key=lambda x: x[class_index]) # Sort by class_index
    cur_fold = 0
    for row in table:
        folds[cur_fold].append(row)
        cur_fold = (cur_fold + 1) % k

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

"""
Takes a table and an index, calculates the minimum and
divisor used to normalize a value, then returns those.
"""
def get_normalize_factors_from_titanic(table, index):
    d = {'crew' : 0., 'first' : 0.33, 'second' : 0.66, 'third' : 1.,
         'child' : 0., 'adult' : 1.,
         'female' : 0., 'male' : 1.}
    mini = min([d[row[index]] for row in table])
    maxi = max([d[row[index]] for row in table])
    return (mini, (maxi - mini) * 1.0)

"""
Takes a training set and an instance, and returns a label
"""
def guess_label_using_stratified_k_fold(training, instance, k):
    import operator
    import math
    
    neighbors = []

    clsmin, clsminmax = get_normalize_factors_from_titanic(training, CLASS)
    agemin, ageminmax = get_normalize_factors_from_titanic(training, AGE)
    sexmin, sexminmax = get_normalize_factors_from_titanic(training, SEX)

    def difference(row1, row2, index, mini, minmax):
        d = {'crew' : 0., 'first' : 0.33, 'second' : 0.66, 'third' : 1.,
         'child' : 0., 'adult' : 1.,
         'female' : 0., 'male' : 1.}
        return ((d[row1[index]] - mini) / minmax) - ((d[row2[index]] - mini) / minmax)

    for row in training:
        # Calculate Euclidean distance on normalized values
        distance = 0.0
        distance += difference(row, instance, CLASS, clsmin, clsminmax) ** 2
        distance += difference(row, instance, AGE, agemin, ageminmax) ** 2
        distance += difference(row, instance, SEX, sexmin, sexminmax) ** 2

        root = math.sqrt(distance)

        neighbors.append([root, row[SURVIVED]]) # Add the distance and label to neighbors

        neighbors.sort(key=lambda x: x[0]) # Sort by root

    # Determine the most occuring label
    labels = {}
    for i in range(k):
        labels[neighbors[i][1]] = labels.get(neighbors[i][1], 0) + 1

    """ max item from dictionary from: http://stackoverflow.com/questions/268272/getting-key-with-maximum-value-in-dictionary """
    return max(labels.iteritems(), key=operator.itemgetter(1))[0]

"""
Performs the k-fold classifier on the Titanic Dataset
"""
def k_fold_part(table):
    from tabulate import tabulate
    print '==========================================='
    print 'TEST 3: Stratified Survival Classifier (Titanic)'
    print '==========================================='
    
    folds = partition_into_folds(table, 10, SURVIVED) # Partitions the data into k folds
    matrix = {'yes' : {'yes' : 0, 'no' : 0},
              'no' : {'yes' : 0, 'no' : 0}}
    for i in range(10):
        training_set, test_set = create_test_and_train_from_folds(folds, i)

        # Test the current fold against the training set
        for instance in test_set:
            predicted = guess_label_using_stratified_k_fold(training_set, instance, 10)
            matrix[instance[SURVIVED]][predicted] += 1

    # Print out the confusion matrix
    tabbed_table = []
    for i in ['yes','no']:
        total = sum([matrix[i][j] for j in ['yes','no']]) * 1.0
        tabbed_table.append([i] +
                            [matrix[i][j] for j in ['yes','no']] +
                            [total] +
                            [100. * matrix[i][i] / (total if total != 0 else 1)])
    print tabulate(tabbed_table, headers = ['SURVIVED', 'yes', 'no', 'Total', 'Recognition (%)'])

    numerator = matrix['yes']['yes'] + matrix['no']['no']
    total = numerator + matrix['yes']['no'] + matrix['no']['yes']
    accuracy = float(numerator) / total

    print '   accuracy:', accuracy, ' error_rate:', (1 - accuracy)

"""
Step3:
"""
def step3(table):
    naive_bayes_part(table)
    k_fold_part(table)
    
""" Main function for this file """
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

    step1(copy.deepcopy(auto_table))
    step2(copy.deepcopy(auto_table))

    titanic_table = read_csv('titanic.txt')[1:100]

    step3(copy.deepcopy(titanic_table))

""" Entry point """
if __name__ == '__main__':
    main()
