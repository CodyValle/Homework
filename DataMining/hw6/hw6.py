""" Good luck! """

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

"""Returns the class frequencies for each attribute value:
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
def decision_tree(table, attrs, class_index, F):
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
        ret.append(trees[i])
    return ret

""" Tests a random forest """
def test_random_forest(forest, test, class_index, class_labels):
    for tree in forest:
        print test_tree(tree, test, class_index, class_labels)

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
Creates a testing set and training set from a list of folds, on the index of the test fold.
"""
def create_test_and_train_from_folds(folds, index):
    training_set = []
    # Build a new training set excluding the current fold (index)
    for j in range(len(folds)):
        if index != j:
            training_set += folds[j]
    return (training_set, folds[index])

def main():
    print 'test_random_forest throws an error. Hopefully you can figure it out.'
    print 'in order to partition a set into stratified thirds, partition_into_folds with k = 3, then create_test_and_train_sets with index = 0.'
    print 'then, the training_set will be exactly 2/3 and the test_set will be exactly 1/3.'

""" To make this an executable """
if __name__ == '__main__':
    main()
