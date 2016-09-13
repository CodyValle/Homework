""" Cody Valle, Andrew Burke, and Katreina Carpenter
    CPSC 310
    hw1.py """

import csv
import sys
from tabulate import tabulate

""" Reads a csv file and returns a table (list of lists) """
def read_csv(filename):
    the_file = open(filename, 'r')
    the_reader = csv.reader(the_file, dialect='excel')
    table = []
    for row in the_reader:
        if len(row) > 0:
            table.append(row)
    the_file.close()
    return table

""" Scans a table and returns a list of indexes corresponding to duplicates """
def scan(table, attributes):
    duplicates = []
    for i in range(len(table)):
        for j in range(i + 1, len(table)):
            dup = True
            for index in attributes:
                if table[i][index] != table[j][index]:
                    dup = False
            if dup:
                duplicates.append(i)
    return duplicates

""" Takes a table and writes its contents to a file in csv format """
def write_to_file(table, filename):
    the_file = open(filename, 'w')
    for row in table:
        for i in range(len(row) - 1):
            the_file.write('"' + str(row[i]) + '",')
        the_file.write('"' + row[-1] + '"\n')

""" Removes duplicates in a table depending on certain attributes """
def pop_dups(table, attributes, out_file):
    duplicates = scan(table, attributes)
    for i in range(1,len(duplicates)+1):
        table.pop(duplicates[-i])
    print "--------------------------------------------------"
    print out_file
    print "--------------------------------------------------"
    print "No. of instances: ", len(table)
    print "Duplicates: ", scan(table, attributes)
    return table

""" Joins the two data sets on car name and model year """
def join(table1, table2):
    for row1 in table1:
        found = False
        for row2 in table2:
            if (row1[0] == row2[8]
            and row1[1] == row2[6]):
                found = True
                row2.append(row1[2])
                break
        if not found:
            table2.append(['NA','NA','NA','NA','NA','NA',row1[1],'NA',row1[0], row1[2]])
    for row in table2:
        if len(row) < 10:
            row.append('NA')
    return table2    

""" Makes a list of floats out of a table column """
def get_column_as_floats(table, index):
    column = []
    for row in table:
        if row[index] != 'NA':
            column.append(float(row[index]))
    return column

""" Gets the minimum of a table """
def minimum(column):
    minimum = column[0]
    for value in column:
        if value < minimum:
            minimum = value
    return minimum

""" Gets the maximum of a table """
def maximum(column):
    maximum = column[0]
    for value in column:
        if value > maximum:
            maximum = value
    return maximum

""" Gets the mid value of a table """
def mid_value(column):
    return (maximum(column) + minimum(column)) / 2.0

""" Gets the average value of a table """
def average (column):
    total = 0.0
    for value in column:
        total = total + value
    return total / len(column)

""" Gets the median value of a table """
def median(column):
    mid_index = len(column) / 2
    if len(column) % 2 == 0:
        return (column[mid_index] + column[mid_index - 1]) / 2.0
    else:
        return column[mid_index]
    
""" Prints statistical data about the table """
def print_data(table, attributes):
    data_table = [] # Stores the data to be printed
    # Get the stats
    for attr,index in attributes:	
	    column = get_column_as_floats(table,index)
	    column.sort()
	    data_table.append([attr, minimum(column), maximum(column), mid_value(column), average(column), median(column)])
		
    # Print out the stats
    print 'Summary Statistics:'
    print tabulate(data_table, headers=['attribute','min','max','mid','avg','med'])

""" Clean up the misspellings on some car names """
def clean_price(table):
    table.sort(key=lambda x: x[0])    	
    table[80][0] = 'chevrolet c10' # 'chevy c10'-> 'chevrolet c10'
    table[81][0] = 'chevrolet c20' # 'chevy c20' -> 'chevrolet c20'
    table[157][0] = 'mazda glc deluxe' # 'maxda glc deluxe' -> 'mazda glc deluxe'
    table[158][0] = 'mazda rx3' # 'maxda rx3' -> 'mazda rx3'
    table[162][0] = 'mercedes-benz 300d' # 'mercedes benz 300d' -> 'mercedes-benz 300d'
    table[249][0] = 'toyota corona mark ii (sw)' # 'toyoto corona mark ii (sw)' -> 'toyota corona mark ii (sw)'
    table[266][0] = 'volkswagen rabbit' # 'vw rabbit' -> 'volkswagen rabbit'
    table[267][0] = 'volkswagen rabbit custom' # 'vw rabbit custom' -> 'volkswagen rabbit custom'
    return table

""" Clean up the misspellings on more car names """
def clean_mpg(table):
    table.sort(key=lambda x: x[8]) # Sort by car name   	
    table[51][8] = 'chevrolet chevelle malibu' # 'chevroelt chevelle malibu' -> 'chevrolet chevelle malibu'
    table[89][8] = 'chevrolet c10' # 'chevy c10'-> 'chevrolet c10'
    table[90][8] = 'chevrolet c20' # 'chevy c20' -> 'chevrolet c20'
    table[191][8] = 'mazda glc deluxe' # 'maxda glc deluxe' -> 'mazda glc deluxe'
    table[192][8] = 'mazda rx3' # 'maxda rx3' -> 'mazda rx3'
    table[196][8] = 'mercedes-benz 300d' # 'mercedes benz 300d' -> 'mercedes-benz 300d'
    table[313][8] = 'volkswagen rabbit' # 'vw rabbit' -> 'volkswagen rabbit'
    table[314][8] = 'volkswagen rabbit custom' # 'vw rabbit custom' -> 'volkswagen rabbit custom'
    
    table.sort(key=lambda x: x[8]) # Sort by car name
    
    return table

""" Fills in most of the missing data: MPG, Horsepower"""
def fill_mpg_and_hp(table):
    """ We want to add in missing MPG data
        by a guesstimate MPG based on cars with
        the same number of cylinders and a similar weight"""
    for element in table:
        if element[0] == 'NA': # Unknown MPG data
            if element[4] == 'NA': # The record must have weight data to continue
                continue
            # Weight must be with five percent
            weight_min = int(element[4]) * .95
            weight_max = int(element[4]) * 1.05
            total = 0.0 # Tracks total MPG
            count = 0 # Tracks number of MPG records
            for row in table: # Find similar rows
                if (row[0] != 'NA' and # Has to have MPG data
                    row[1] == element[1] and # Same number of cylinders
                    int(row[4]) >= weight_min and int(row[4]) <= weight_max): # Must be within weight
                    total = total + float(row[0]) # Add the total MPG
                    count = count + 1 # Total number of found records
            if count == 0: # Prevent divide by zero
                print 'Error: No similar cars. Popping.'
                print element
                del element # Remove element we cannot guess data for
            else:
                element[0] = str(total / count) # Place the guessed MPG data

    """ We want to add in missing Horsepower data
        by a guesstimate Horsepower based on cars with
        the same number of cylinders and a similar displacement"""
    for element in table:
        if element[3] == 'NA': # Unknown Horsepower data
            if element[2] == 'NA': # The record must have displacement data to continue
                continue
            # Displacement must be with five percent
            disp_min = float(element[2]) * .95
            disp_max = float(element[2]) * 1.05
            total = 0.0 # Tracks total similar Horsepower records
            count = 0 # Tracks number of similar Horsepower records
            for row in table: # Find similar records
                if (row[3] != 'NA' and # Has to have Horsepower data
                    row[1] == element[1] and # Same number of cylinders
                    float(row[2]) >= disp_min and float(row[2]) <= disp_max): # Must be within weight
                    total = total + float(row[3]) # Add the total Horsepower
                    count = count + 1 # Total number of found records
            if count == 0: # Prevent divide by zero
                print 'Error: No similar cars. Popping.'
                print element
                del element # Remove element we cannot guess data for
            else:
                element[3] = str(total / count) # Place the guessed Horsepower data
    return table
    
""" Removes all rows where missing data is present and prints a summary """
def remove_missing_data_rows(table):
	# Collect the indices that need to be deleted
	missing = []
	for i in range(len(table)):
		for element in table[i]:
			if element == 'NA':
				missing.append(i)
				break;
	
	# Delete the rows
	for i in range(1, len(missing) + 1):
		table.pop(missing[-i])
	
	# Print out sumary stats
	pop_dups(table, [6,8],'combined table (w/ rows missing data removed)')
	print_data(table, [['mpg',0],['disp',2],['hp',3],['weight',4],['accel',5],['msrp',9]])
	
""" Fills in missing data with the average of that column, then prints a summary """
def fill_data_with_average(table):
	mpg_average = average(get_column_as_floats(table,0))
	hp_average = average(get_column_as_floats(table,3))
	msrp_average = average(get_column_as_floats(table,9))
	
	# Fill in the data
	for row in table:
		if row[0] == 'NA':
			row[0] = mpg_average
		if row[3] == 'NA':
			row[3] = hp_average
		if row[9] == 'NA':
			row[9] = msrp_average
		
	# Print out sumary stats
	pop_dups(table, [6,8],'combined table (w/ rows missing data filled with averages)')
	print_data(table, [['mpg',0],['disp',2],['hp',3],['weight',4],['accel',5],['msrp',9]])

""" Fills mising data with similar values from the table, then prints a summary """	
def fill_data_with_similar(table):
    table =	fill_mpg_and_hp(table) # Fills in mpg and hp data, removes undeterminable rows

    """ We want to add in missing MSRP data
    by a guesstimate MSRP based on cars from
    the same year, similar weight and acceleration """
    subset = [] # Gather a subset of possible data points
    for row in table:
        if (row[9] != 'NA' and # The record must have msrp data to continue
            row[6] != 'NA' and # The record must have year data to continue
            row[4] != 'NA' and # The record must have weight data to continue
            row[5] != 'NA'): # The record must have accel data to continue
            subset.append(row)
  
    to_fill = [] # Gather the rows that must be filled
    for row in table:
        if row[9] == 'NA': # Unknown MSRP data
            if (row[6] != 'NA' and # The record must have year data to continue
                row[4] != 'NA' and # The record must have weight data to continue
                row[5] != 'NA'): # The record must have accel data to continue
                to_fill.append(row)

    """ Fill in the missing data """
    pop_rows = []
    for j in range(len(to_fill)):
        sim_year = [] # Gather records from the same year
        year_min = int(table[j][6]) - 2
        year_max = year_min + 4
        for i in subset:
            if year_min <= int(i[6]) and int(i[6]) <= year_max:
                sim_year.append(i)

	sim_weight = [] # Gather records of similar weight
	weight_min = float(table[j][4]) * 0.95
	weight_max = float(table[j][4]) * 1.05
	for i in sim_year:
            if weight_min <= float(i[4]) and float(i[4]) <= weight_max:
                sim_weight.append(i)

	sim_accel = [] # Gather records of similar acceleration
	accel_min = float(table[j][5]) * 0.9
	accel_max = float(table[j][5]) * 1.1
	for i in sim_weight:
            if accel_min <= float(i[5]) and float(i[5]) <= accel_max:
                sim_accel.append(i)

        # Average msrp and assign to row if possible
        if len(sim_accel) > 0:
            row[9] = average(get_column_as_floats(sim_accel, 9))
        else:
            pop_rows.append(j)

    # Remove rows we couldnt determine
    for i in range(1, len(pop_rows) + 1):
        table.pop(pop_rows[-i])
	
    # Print out sumary stats
    pop_dups(table, [6,8],'combined table (w/ rows missing data filled with averages)')
    print_data(table, [['mpg',0],['disp',2],['hp',3],['weight',4],['accel',5],['msrp',9]])

""" The main function """
def main():
    price_table = read_csv('auto-prices.txt')
    mpg_table = read_csv('auto-mpg.txt')
    
    """ Remove extraneous rows that pollute the real data """
    mpg_table.sort(key=lambda x: x[0]) # Sorts on MPG
    del mpg_table[-8] # Removes ['9.0', '8', '304.0', '193.0', '4732', '18.5', '70', '1', 'hi 1200d']
    
    """ Fix misspellings on some car names """
    price_table = clean_price(price_table)
    mpg_table = clean_mpg(mpg_table)


    """ Remove duplicate rows and prints data about the table """
    price_table = pop_dups(price_table, [0,1], 'auto-prices-nodups.txt')
    mpg_table = pop_dups(mpg_table, [6,8], 'auto-mpg-nodups.txt')
    
    """ Fill missing data with similar values """
    mpg_table = fill_mpg_and_hp(mpg_table)

    """ Write tables to files """
    write_to_file(price_table, 'auto-prices-clean.txt')
    write_to_file(mpg_table, 'auto-mpg-clean.txt')

    """ Join the tables """
    joined_table = join(price_table, mpg_table)

    """ Remove duplicate rows and prints data about the table """
    joined_table = pop_dups(joined_table, [6,8], 'combined table (saved as auto-data.txt)')

    """ Prints statistical summary data of the table """
    print_data(joined_table, [['mpg',0],['disp',2],['hp',3],['weight',4],['accel',5],['msrp',9]])

    """ Write the final table to file """
    write_to_file(joined_table, 'auto-data.txt')
    
    """ Removes all rows that contain missing values and prints a summary """
    remove_missing_data_rows(read_csv('auto-data.txt'))
    
    """ Replaces missing data with an average, then prints a summary """
    fill_data_with_average(read_csv('auto-data.txt'))
    
    """ Fill missing data with similar values """
    fill_data_with_similar(read_csv('auto-data.txt'))
    
if __name__ == '__main__':
    main()
