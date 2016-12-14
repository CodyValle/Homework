from tabulate import tabulate

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
            
        l += [round(row[AIR_TEMP], 1) if row[AIR_TEMP] else None,
              round(row[SEA_TEMP], 1) if row[SEA_TEMP] else None]
        
        categorized.append(tuple(l))
        index += 1
        
    return categorized

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
Main function. Loads the dataset, performs summary statistics, creates plots,
creates and compares classifiers to guess temperatures based on weather and date.
"""
def main():
    elnino, atts = load_elnino()
    elnino, fill = clean(elnino, [YEAR, MONTH, LATITUDE, LONGITUDE, HUMIDITY, AIR_TEMP, SEA_TEMP])
    elnino = categorize(elnino)

    import myPlots
    myPlots.do_plots(elnino, True)

    import myClassifiers
    #elnino = bootstrap(elnino, 300)
    myClassifiers.compare_classifiers(elnino, atts)

""" To make this an executable """
if __name__ == '__main__':
    main()
