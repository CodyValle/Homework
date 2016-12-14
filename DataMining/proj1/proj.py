# Import useful functions
from myUtil import *

# Indexes of this dataset
global OBSERVATION; OBSERVATION = 0
global YEAR;        YEAR        = 1
global MONTH;       MONTH       = 2
global DAY;         DAY         = 3
global LATITUDE;    LATITUDE    = 4
global LONGITUDE;   LONGITUDE   = 5
global ZONAL;       ZONAL       = 6
global MERIDIONAL;  MERIDIONAL  = 7
global HUMIDITY;    HUMIDITY    = 8
global AIR_TEMP;    AIR_TEMP    = 9
global SEA_TEMP;    SEA_TEMP    = 10

""" Tests a random forest """
def test_random_forest(forest, test, class_index, class_labels):
    for tree in forest:
        print test_tree(tree, test, class_index, class_labels)

""" Loads the El Nino Dataset, and returns the attributes and converted rows. """
def load_elnino():
    import csv
    with open('elnino.csv', 'r') as the_file:
        the_reader = csv.reader(the_file, dialect='excel')
        atts = the_reader.next()
        del atts[4]
        table = [tuple([int(i) for i in row[:4]] +
                       [float(i) for i in row[5:7]] +
                       [float(i) if i != '.' else None for i in row[7:12]])
                 for row in the_reader if len(row) > 0]
        return table, atts

""" Categorizes the values in the El Nino dataset """
def categorize(table):
    table.sort(key = lambda x:x[HUMIDITY])
    categorized = []
    for index, row in enumerate(table):
        l = [row[OBSERVATION], row[YEAR], row[MONTH], row[DAY]]
        
        l += [round(row[LATITUDE], 0),
              round(row[LONGITUDE], 0)]
        
        l += [round(row[ZONAL], 0),
              round(row[MERIDIONAL], 0)]

        """ These indices were determined as the best split points using entropy """
        if index <= 12700:
            l += [0]
        elif index <= 22404:
            l += [1]
        elif index <= 68315:
            l += [2]
        elif index <= 33514:
            l += [3]
        elif index <= 46139:
            l += [4]
        elif index <= 58407:
            l += [5]
        elif index <= 70118:
            l += [6]
        elif index <= 79184:
            l += [7]
        elif index <= 92999:
            l += [8]
        elif index <= 92999:
            l += [9]
        elif index <= 100131:
            l += [10]
        else:
            l += [11]
            
        l += [round(row[AIR_TEMP], 0),
              round(row[SEA_TEMP], 0)]
        
        categorized.append(tuple(l))
        
    return categorized

"""
Main function. Loads the dataset, performs summary statistics, creates plots,
creates and compares classifiers to guess temperatures based on weather and date.
"""
def main():
    elnino, atts = load_elnino()
    elnino, fill = clean(elnino, [YEAR, MONTH, LATITUDE, LONGITUDE, HUMIDITY, ZONAL, MERIDIONAL, AIR_TEMP, SEA_TEMP])
    elnino_categorized = categorize(elnino)

    # Print summary statistics
    #summary_statistics(elnino, atts, [ZONAL, MERIDIONAL, HUMIDITY, AIR_TEMP, SEA_TEMP])

    import myPlots
    myPlots.do_plots(elnino, True)

    import myClassifiers
    #elnino = bootstrap(elnino, 300)
    myClassifiers.compare_classifiers(elnino, atts)

""" To make this an executable """
if __name__ == '__main__':
    main()
