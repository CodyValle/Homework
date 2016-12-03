from tabulate import tabulate
import matplotlib.pylab as plt
import numpy as np
import math
import copy
import random

# Indexes of this dataset
global OBSERVATION
OBSERVATION = 0
global YEAR
YEAR = 1
global MONTH
MONTH = 2
global DAY
DAY = 3
global LATITUDE
LATITUDE = 4
global LONGITUDE
LONGITUDE = 5
global ZONAL
ZONAL = 6
global MERIDIONAL
MERIDIONAL = 7
global HUMIDITY
HUMIDITY = 8
global AIR_TEMP
AIR_TEMP = 9
global SEA_TEMP
SEA_TEMP = 10

""" Loads the El Nino Dataset, and returns the attributes and converted rows. """
def load_elnino():
    import csv
    the_file = open('elnino.csv', 'r')
    the_reader = csv.reader(the_file, dialect='excel')
    table = []
    atts = the_reader.next()
    del atts[4]
    for row in the_reader:
        if len(row) > 0:
            table.append(tuple([int(i) for i in row[:4]] +
                         [float(i) for i in row[5:7]] +
                         [float(i) if i != '.' else None for i in row[7:12]]))
    the_file.close()
    return table, atts

""" Cleans the table of all rows with at least one None """
def clean(table, indices = None):
    keep = []
    rem = []
    if indices == None:
        indices = list(range(len(table[0])))
    for row in table:
        good = True
        for index in indices:
            if row[index] == None:
                rem.append(row)
                good = False
                break
        if good:
            keep.append(row)
    return keep, rem

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

""" Strips a table of all rows on an index that are None """
def strip(table, index):
    ret = []
    for row in table:
        if row[index] != None:
            ret.append(row)
    return ret

""" Creates a bootstrap set """
def bootstrap(table, number = None):
    if number == None:
        number = len(table)
    ret = []
    for _ in range(number):
        ret.append(random.choice(table))
    return ret

"""
Performs summary statistics on the El Nino Dataset.
Reports mins and maxes of temperatures, wind strength, and humidity.
"""
def summary_statistics(table, atts):
    import operator
    
    # Needed minimums, maximumn, sums, counts, and modes
    zonal_min = table[0][ZONAL]
    zonal_max = table[0][ZONAL]
    zonal_sum = 0
    zonal_count = 0
    zonal_mode_dict = {}
    merid_min = table[0][MERIDIONAL]
    merid_max = table[0][MERIDIONAL]
    merid_sum = 0
    merid_count = 0
    merid_mode_dict = {}
    humid_min = 101. # Special case, table[0][HUMIDITY] = None
    humid_max = table[0][HUMIDITY]
    humid_sum = 0
    humid_count = 0
    humid_mode_dict = {}
    air_min = table[0][AIR_TEMP]
    air_max = table[0][AIR_TEMP]
    air_sum = 0
    air_count = 0
    air_mode_dict = {}
    sea_min = table[0][SEA_TEMP]
    sea_max = table[0][SEA_TEMP]
    sea_sum = 0
    sea_count = 0
    sea_mode_dict = {}

    # Fill the needed values
    for row in table[1:]:
        if row[ZONAL] != None:
            if row[ZONAL] < zonal_min:
                zonal_min = row[ZONAL]
            if row[ZONAL] > zonal_max:
                zonal_max = row[ZONAL]
            zonal_sum += row[ZONAL]
            zonal_count += 1
            zonal_mode_dict[row[ZONAL]] = zonal_mode_dict.get(row[ZONAL], 0) + 1
            
        if row[MERIDIONAL] != None:
            if row[MERIDIONAL] < merid_min:
                merid_min = row[MERIDIONAL]
            if row[MERIDIONAL] > merid_max:
                merid_max = row[MERIDIONAL]
            merid_sum += row[MERIDIONAL]
            merid_count += 1
            merid_mode_dict[row[MERIDIONAL]] = merid_mode_dict.get(row[MERIDIONAL], 0) + 1
            
        if row[HUMIDITY] != None:
            if row[HUMIDITY] < humid_min:
                humid_min = row[HUMIDITY]
            if row[HUMIDITY] > humid_max:
                humid_max = row[HUMIDITY]
            humid_sum += row[HUMIDITY]
            humid_count += 1
            humid_mode_dict[row[HUMIDITY]] = humid_mode_dict.get(row[HUMIDITY], 0) + 1
            
        if row[AIR_TEMP] != None:
            if row[AIR_TEMP] < air_min:
                air_min = row[AIR_TEMP]
            if row[AIR_TEMP] > air_max:
                air_max = row[AIR_TEMP]
            air_sum += row[AIR_TEMP]
            air_count += 1
            air_mode_dict[row[AIR_TEMP]] = air_mode_dict.get(row[AIR_TEMP], 0) + 1
            
        if row[SEA_TEMP] != None:
            if row[SEA_TEMP] < sea_min:
                sea_min = row[SEA_TEMP]
            if row[SEA_TEMP] > sea_max:
                sea_max = row[SEA_TEMP]
            sea_sum += row[SEA_TEMP]
            sea_count += 1
            sea_mode_dict[row[SEA_TEMP]] = sea_mode_dict.get(row[SEA_TEMP], 0) + 1

    # Get the modes
    zonal_mode = max(zonal_mode_dict.iteritems(), key=operator.itemgetter(1))[0]
    merid_mode = max(merid_mode_dict.iteritems(), key=operator.itemgetter(1))[0]
    humid_mode = max(humid_mode_dict.iteritems(), key=operator.itemgetter(1))[0]
    air_mode = max(air_mode_dict.iteritems(), key=operator.itemgetter(1))[0]
    sea_mode = max(sea_mode_dict.iteritems(), key=operator.itemgetter(1))[0]

    # Calculate medians
    table.sort(key=lambda x: x[ZONAL])
    med_index = -zonal_count / 2 + len(table) - 1
    zonal_med = table[med_index][ZONAL]
    if zonal_count % 2 == 1:
        zonal_med += table[med_index + 1][ZONAL]
        zonal_med /= 2.
        
    table.sort(key=lambda x: x[MERIDIONAL])
    med_index = -merid_count / 2 + len(table) - 1
    merid_med = table[med_index][MERIDIONAL]
    if merid_count % 2 == 1:
        merid_med += table[med_index + 1][MERIDIONAL]
        merid_med /= 2.
        
    table.sort(key=lambda x: x[HUMIDITY])
    med_index = -humid_count / 2 + len(table) - 1
    humid_med = table[med_index][HUMIDITY]
    if humid_count % 2 == 1:
        humid_med += table[med_index + 1][HUMIDITY]
        humid_med /= 2.
        
    table.sort(key=lambda x: x[AIR_TEMP])
    med_index = -air_count / 2 + len(table) - 1
    air_med = table[med_index][AIR_TEMP]
    if air_count % 2 == 1:
        air_med += table[med_index + 1][AIR_TEMP]
        air_med /= 2.
        
    table.sort(key=lambda x: x[SEA_TEMP])
    med_index = -sea_count / 2 + len(table) - 1
    sea_med = table[med_index][SEA_TEMP]
    if sea_count % 2 == 1:
        sea_med += table[med_index + 1][SEA_TEMP]
        sea_med /= 2.

    # Print summary statistics
    print 'Summary Statistics'
    tab_table = []
    tab_table.append([atts[ZONAL], zonal_min, zonal_max, float(zonal_sum) / zonal_count, zonal_med, zonal_mode])
    tab_table.append([atts[MERIDIONAL], merid_min, merid_max, float(merid_sum) / merid_count, merid_med, merid_mode])
    tab_table.append([atts[HUMIDITY], humid_min, humid_max, float(humid_sum) / humid_count, humid_med, humid_mode])
    tab_table.append([atts[AIR_TEMP], air_min, air_max, float(air_sum) / air_count, air_med, air_mode])
    tab_table.append([atts[SEA_TEMP], sea_min, sea_max, float(sea_sum) / sea_count, sea_med, sea_mode])
    print tabulate(tab_table, headers = ['Metric', 'Min', 'Max', 'Avg', 'Med', 'Mode'])

"""
Splits the elnino dataset into the separate buoys
BROKEN
"""
def split_into_buoys(instances):
    """
    lats = list(set([int(x[LATITUDE] + 0.5) for x in instances]))
    longs = list(set([int(x[LONGITUDE] + 0.5) for x in instances]))

    lats.sort()
    longs.sort()

    print 'Latitudes:'
    for lat in lats:
        print lat

    print 'Longitudes:'
    for long in longs:
        print long

    coords = list(set([(int(x[LATITUDE] + 0.5), int(x[LONGITUDE] + 0.5)) for x in instances]))

    coords.sort(key=lambda x: x[1])
    coords.sort(key=lambda x: x[0])

    print 'Coordinates:'
    for coord in coords:
        print coord
    """

    tups = list(set([(x[LATITUDE], x[LONGITUDE]) for x in instances]))

    tups.sort(key=lambda x: x[1])
    tups.sort(key=lambda x: x[0])

    print 'Min:',tups[0],'Max:',tups[-1]

""" Creates a plot of the locations of the buoys """
def buoy_location_plot(table):
    # We modulate longitude because coordinates wrap at -180 and 180
    coords = [(x[LONGITUDE] % 360, x[LATITUDE]) for x in table]
    xs = []
    ys = []

    for coord in coords:
        xs.append(coord[0])
        ys.append(coord[1])

    plt.figure() # Reset the figure
    plt.suptitle('Buoy Coordinates', fontsize=12, fontweight='bold')
    plt.grid(True) # Turn on the grid

    plt.scatter(xs, ys, c='g') # Plot the data

    # we have to override the actual numbers with normal data
    xlabels = [120, 140, 160, 0, -160, -140, -120, -100]
    plt.xticks(range(120, 270, 20), xlabels)
    plt.yticks(range(-10, 10, 2))
    
    plt.xlim(min(xs) - 5, max(xs) + 5) # Set the x limits
    plt.ylim(min(ys) - 0.2, max(ys) + 0.2) # Set the y limits
    
    plt.xlabel('Longitude')
    plt.ylabel('Latitude')

    plt.show()
    plt.close() # Clean up

"""
Creates various plots of the data
"""
def do_plots(table):
    buoy_location_plot(table)

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

""" Categorizes the values in the El Nino dataset """
def categorize(table, sea_temp = True):
    table.sort(key = lambda x:x[HUMIDITY])
    categorized = []
    index = 0
    for row in table:
        l = [row[OBSERVATION], row[YEAR], row[MONTH], row[DAY], row[LATITUDE], row[LONGITUDE],
               row[ZONAL], row[MERIDIONAL]]

        """ These indices were determined as the best split points using entropy """
        """
        if sea_temp: # Categorize the humidity for sea temperatures
            if index <= 22991:
                l += [0]
            elif index <= 35192:
                l += [1]
            elif index <= 68315:
                l += [2]
            elif index <= 99657:
                l += [3]
            elif index <= 100392:
                l += [4]
            else:
                l += [5]
                
        else: # Categorize the humidity for air temperatures
            if index <= 56595:
                l += [0]
            elif index <= 65979:
                l += [1]
            elif index <= 68751:
                l += [2]
            else:
                l += [3]
        """
        l += [row[HUMIDITY]]
            
        l += [row[AIR_TEMP],
              round(row[SEA_TEMP], 0) if row[SEA_TEMP] != None else None]
        
        categorized.append(tuple(l))
        index += 1
        
    return categorized

""" Classifies the dataset """
def classify(table, atts):
    categorized = categorize(table) # Categorizes the data
    
    folds = partition_into_folds(categorized, 3, SEA_TEMP)

    sea_temps = get_categories(categorized, SEA_TEMP)
    if None in sea_temps:
        sea_temps.pop(sea_temps.index(None))

    correct = 0
    total = 0
    for i in range(3):
        training_set, test_set = create_test_and_train_from_folds(folds, i)
        tree = decision_tree(training_set, [YEAR, MONTH], SEA_TEMP)
        for row in test_set:
            if row[SEA_TEMP] == None:
                continue
            predicted = decide(tree, row, sea_temps)
            if row[SEA_TEMP] == predicted:
                correct += 1
            total += 1

    print 'Accuracy:', float(correct) / total

""" Does work in a thread for the determine_split_points_threadpool function """
def split_points_worker_threadpool(arg):
    table = arg[0]
    att_index = arg[1]
    class_index = arg[2]
    labels = arg[3]
    start_index = arg[4]
    stop_index = arg[5]
    
    lowest = calc_enew(table[:start_index + 2], att_index, class_index, labels) * (start_index + 2)
    lowest += calc_enew(table[start_index + 2:], att_index, class_index, labels) * len(table[start_index + 2:])
    split = 2
    for i in range(start_index + 3, stop_index):
        # Calculate Enew
        enew = calc_enew(table[:i], att_index, class_index, labels) * len(table[:i])
        enew += calc_enew(table[i:], att_index, class_index, labels) * len(table[i:])

        # Update if needed
        if enew < lowest:
            split = i
            lowest = enew

    return (lowest, split)

""" Does work in a thread for the determine_split_points_threadsfunction """  
def split_points_worker_threads(table, att_index, class_index, labels, start_index, stop_index, result):
    lowest = calc_enew(table[:start_index + 2], att_index, class_index, labels) * (start_index + 2)
    lowest += calc_enew(table[start_index + 2:], att_index, class_index, labels) * len(table[start_index + 2:])
    split = 2
    for i in range(start_index + 3, stop_index):
        # Calculate Enew
        enew = calc_enew(table[:i], att_index, class_index, labels) * len(table[:i])
        enew += calc_enew(table[i:], att_index, class_index, labels) * len(table[i:])

        # Update if needed
        if enew < lowest:
            split = i
            lowest = enew

    result[start_index] = (lowest, split)

""" Finds the best index to split the data """
def split_points(table, att_index, class_index, n):
    table.sort(key=lambda x:x[att_index])
    labels = get_categories(table, class_index)

    lowest = calc_enew(table[:2], att_index, class_index, labels) * 2
    lowest += calc_enew(table[2:], att_index, class_index, labels) * (len(table) - 2)
    split = 2
    for i in range(3, len(table) - 2):
        # Calculate Enew
        enew = calc_enew(table[:i], att_index, class_index, labels) * len(table[:i])
        enew += calc_enew(table[i:], att_index, class_index, labels) * len(table[i:])

        # Update if needed
        if enew < lowest:
            split = i
            lowest = enew

    ret = [split]
    if n > 1:
        before = split_points(table[:split], att_index, class_index, n - 1)
        ret = before + ret
        
        after = split_points(table[split:], att_index, class_index, n - 1)
        for each in after:
            ret += [each + split]

    return ret

""" Determines the best split points for the dataset using threads """
def determine_split_points_threads(table):
    #elnino, atts = load_elnino()
    #table = elnino[:100]
    
    #data, fill = clean(table, [YEAR, MONTH, LATITUDE, LONGITUDE, HUMIDITY, AIR_TEMP, SEA_TEMP])
    table = categorize(table)
    print 'Rows:', len(table)

    table.sort(key=lambda x:x[HUMIDITY])
    labels = get_categories(table, SEA_TEMP)

    from threading import Thread
    
    num_threads = 8

    results = {}
    args = []
    l = float(len(table))
    for i in range(num_threads):
        lst = [table, HUMIDITY, SEA_TEMP, labels]
        lst += [int(l / num_threads * i + .5)]
        lst += [int(l / num_threads * (i + 1) + .5)]
        lst += [results]
        args.append(tuple(lst))

    threads = []
    for arg in args:
        t = Thread(target=split_points_worker_threads, args=arg)
        t.start()
        threads.append(t)
        
    for thread in threads:
        thread.join()

    results = results.values()
    results.sort(key=lambda x:x[0])
    for r in results:
        print r
        
    print 'Done!'

""" Determines the best split points for the dataset using dummy threads """
def determine_split_points_threadpool(table):
    #elnino, atts = load_elnino()
    #table = elnino[:100]
    
    #data, fill = clean(table, [YEAR, MONTH, LATITUDE, LONGITUDE, HUMIDITY, AIR_TEMP, SEA_TEMP])
    table = categorize(table)
    print 'Rows:', len(table)

    table.sort(key=lambda x:x[HUMIDITY])
    labels = get_categories(table, SEA_TEMP)

    from multiprocessing.dummy import Pool as ThreadPool
    num_threads = 8
    
    pool = ThreadPool(num_threads)

    args = []
    l = float(len(table))
    for i in range(num_threads):
        lst = [table, HUMIDITY, SEA_TEMP, labels]
        lst += [int(l / num_threads * i + .5)]
        lst += [int(l / num_threads * (i + 1) + .5)]
        args.append(tuple(lst))

    results = pool.map(split_points_worker_threadpool, args)

    pool.close()
    pool.join()

    results.sort(key=lambda x:x[0])

    for r in results:
        print r
    """
(268109.6362056146, 99657)
(268121.6767270988, 68315)
(268123.24279139587, 81654)
(268124.24775218713, 92224)
(268134.375379706, 93281)
(268134.89608269086, 35192)
(268135.5173712733, 62152)
(268138.51301281207, 22991)
(268141.0542846892, 29999)
(268142.58684238617, 53175)
    """
    print 'Done!'

def wrapper(func, *args, **kwargs):
    def wrapped():
        return func(*args, **kwargs)
    return wrapped

"""
Main function. Loads the dataset, performs summary statistics,
creates classifiers to guess temperatures based on weather and date.
"""
def main():
    elnino, atts = load_elnino()
    elnino, fill = clean(elnino, [YEAR, MONTH, LATITUDE, LONGITUDE, HUMIDITY, AIR_TEMP, SEA_TEMP])
    elnino = bootstrap(elnino, 30000)

    import cProfile, pstats
    
    prof = cProfile.Profile() # Create the profiler
    prof.enable() # Start the profiler
    determine_split_points_threadpool(elnino)
    prof.disable() # Stop the profiler
    poolStats = pstats.Stats(prof).sort_stats('cumtime') # Sort by time in function
    
    prof = cProfile.Profile() # Create new profiler
    prof.enable() # Start the profiler
    determine_split_points_threads(elnino)
    prof.disable() # Stop the profiler
    threadStats = pstats.Stats(prof).sort_stats('cumtime') # Sort by time in function
    
    poolStats.print_stats('proj.py') # Restrict stats to functions called in this file
    threadStats.print_stats('proj.py') # Restrict stats to functions called in this file
    """
    prof = cProfile.Profile() # Create new profiler
    prof.enable() # Start the profiler
    split_points(elnino, HUMIDITY, SEA_TEMP, 1)
    prof.disable() # Stop the profiler
    rawStats = pstats.Stats(prof).sort_stats('cumtime') # Sort by time in function
    
    rawStats.print_stats('proj.py') # Restrict stats to functions called in this file
    """
    """
    chunk, fill = clean(elnino, [YEAR, MONTH, LATITUDE, LONGITUDE, HUMIDITY, AIR_TEMP, SEA_TEMP])
    chunk = bootstrap(chunk, 300)
    chunk = categorize(chunk)
    sea_temps = get_categories(chunk, SEA_TEMP)

    ensemble = random_forest(bootstrap(chunk, 200),
                             [YEAR, MONTH, LATITUDE, LONGITUDE, HUMIDITY, AIR_TEMP],
                             SEA_TEMP,
                             sea_temps,
                             100,
                             200,
                             2)
    test_random_forest(ensemble, bootstrap(chunk, 100), SEA_TEMP, sea_temps)

    prev_tree = decision_tree(chunk, [YEAR, MONTH, LATITUDE, LONGITUDE, HUMIDITY, AIR_TEMP], SEA_TEMP, 2)
    for _ in range(3):
        tree = decision_tree(chunk, [YEAR, MONTH, LATITUDE, LONGITUDE, HUMIDITY, AIR_TEMP], SEA_TEMP, 2)
        print tree == prev_tree
        prev_tree = tree
        row = bootstrap(elnino, 1)[0]
        print decide(tree, row, sea_temps), row[SEA_TEMP]
    """
    """
    folds = partition_into_folds(chunk, 10, SEA_TEMP)
    sea_temps = get_categories(chunk, SEA_TEMP)
    correct = 0
    total = 0
    for i in range(len(folds)):
        training_set, test_set = create_test_and_train_from_folds(folds, i)
        tree = decision_tree(training_set, [YEAR, MONTH], SEA_TEMP)
        for row in test_set:
            predicted = decide(tree, row, sea_temps)
            if row[SEA_TEMP] == predicted:
                correct += 1
            total += 1
    print 'Correct:', correct
    print 'Total:', total
    print 'Accuracy:', float(correct) / total
    """
    
    #determine_split_points(elnino)
    #summary_statistics(elnino, atts)
    #split_into_buoys(elnino)
    #do_plots(elnino)
    #classify(elnino, atts)

""" To make this an executable """
if __name__ == '__main__':
    main()

