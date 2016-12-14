""" Creates a Decision Tree based on the passed in attributes """
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
    return [trees[i] for i in range(-1, -M - 1, -1)]

""" Linear regression approach """
def linear_regression_approach(table):
    pass

""" k-NN approach """
def k_nn_approach(table):
    pass

""" Decision tree approach """
def decision_tree_approach(table):
    pass

""" Random Forest approach """
def random_forest_approach(table):
    pass

""" Compares various classifiers """
def compare_classifiers(table, att_names):
    linear_regression_approach(table)
    k_nn_approach(table)
    decision_tree_approach(table)
    random_forest_approach(table)
