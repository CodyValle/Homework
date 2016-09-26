from collections import defaultdict
import matplotlib.pylab as plt
import numpy as np

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

""" Helper function for step1 """
def doPlot1(column, strings):
    dict = {} # Initialize a dictionary
    
    for i in column: # Count the number of cars with each number of cylinders
        dict[i] = dict.get(i, 0) + 1 # Increase the count or create and increase

    xs = []
    ys = []
    for key, val in dict.items(): # Iterate through, and create values for the plot
        xs.append(key)
        ys.append(val)

    plt.figure() # Reset the figure
    plt.suptitle(strings['title'], fontsize=12, fontweight='bold')

    plt.bar(xs, ys, 1/1.5, color='blue', align='center') # Plot the data
    plt.xlabel(strings['xlabel'])
    plt.ylabel(strings['ylabel'])
    plt.savefig('step-1-' + strings['filename'] + '.pdf') # Save the figure
    if show_graphs:
        plt.show()
    plt.close()

""" Performs step 1 of the homework """
def step1(table):
    column = get_column_as_floats(table, 1) # Get the cylinders
    strings = {'title' : 'Number of Cars versus Number of Cylinders',
               'xlabel' : 'Number of Cylinders',
               'ylabel' : 'Number of Cars',
               'filename' : 'cylinders'}
    doPlot1(column, strings) # Plot the cylinders
    
    column = get_column_as_floats(table, 6) # Get the model year
    strings = {'title' : 'Number of Cars versus Model Year',
               'xlabel' : 'Model Year',
               'ylabel' : 'Number of Cars',
               'filename' : 'model_year'}
    doPlot1(column, strings) # Plot the model year
    
    column = get_column_as_floats(table, 7) # Get the origin
    strings = {'title' : 'Number of Cars versus Origin',
               'xlabel' : 'Origin',
               'ylabel' : 'Number of Cars',
               'filename' : 'origin'}
    doPlot1(column, strings) # Plot the origin

""" Helper function for step5 """
def doPlot5(column, strings):
    plt.figure()
    plt.suptitle(strings['title'], fontsize=12, fontweight='bold')

    plt.hist(column, color='green', align = 'left') # Plot the data
    plt.xlabel(strings['xlabel'])
    plt.ylabel(strings['ylabel'])
    plt.savefig('step-5-' + strings['filename'] + '.pdf') # Save the figure
    if show_graphs:
        plt.show()
    plt.close()

""" Performs step 5 of the homework """
def step5(table):
    column = get_column_as_floats(table, 0) # Get the MPG attribute
    strings = {'title' : 'Number of Cars versus Miles Per Gallon',
               'xlabel' : 'Miles Per Gallon',
               'ylabel' : 'Number of Cars',
               'filename' : 'mpg'}
    doPlot5(column, strings)
    
    column = get_column_as_floats(table, 2) # Get the displacement attribute
    strings = {'title' : 'Number of Cars versus Displacement',
               'xlabel' : 'Displacement',
               'ylabel' : 'Number of Cars',
               'filename' : 'disp'}
    doPlot5(column, strings)
    
    column = get_column_as_floats(table, 3) # Get the horsepower attribute
    strings = {'title' : 'Number of Cars versus Horsepower',
               'xlabel' : 'Horsepower',
               'ylabel' : 'Number of Cars',
               'filename' : 'hp'}
    doPlot5(column, strings)
    
    column = get_column_as_floats(table, 4) # Get the weight attribute
    strings = {'title' : 'Number of Cars versus Weight',
               'xlabel' : 'Weight',
               'ylabel' : 'Number of Cars',
               'filename' : 'weight'}
    doPlot5(column, strings)
    
    column = get_column_as_floats(table, 5) # Get the acceleration attribute
    strings = {'title' : 'Number of Cars versus Acceleration',
               'xlabel' : 'Acceleration',
               'ylabel' : 'Number of Cars',
               'filename' : 'accel'}
    doPlot5(column, strings)

""" Performs step 8 of the homework """
def step8(table):
    table.sort(key=lambda x: x[6]) # Sort on model year
    
    data = []
    years = []
    element = []
    cur_year = table[0][6]
    for row in table:
        if row[0] == 'NA' or row[6] == 'NA':
            continue
        if row[6] == cur_year:
            element.append(float(row[0])) # Add MPG data
        else:
            data.append(element)
            element = []
            years.append(cur_year)
            cur_year = row[6]
    data.append(element)
    years.append(cur_year)
    
    fig = plt.figure()
    plt.suptitle('Miles Per Gallon vs Model Year', fontsize=12, fontweight='bold')
    ax = fig.add_subplot(111)
    ax.boxplot(data)
    ax.set_xticklabels(years)
    plt.xlabel('Model Year') # Set x axis label
    plt.ylabel('Miles Per Gallon') # Set y axis label
    fig.savefig('step-8-chart1.pdf') # Save the figure
    if show_graphs: # Show the graph if preferred
        plt.show()
    plt.close() # Clean up

    """ Draw the second chart, a multi-frequency graph of
        the number of cars produced by year and country. """
    years = [] # Will hold the year labels that align with the following data lists
    USAdata = [0] # Will hold the number of cars produced by USA each year
    EURdata = [0] #  "    "    " ...                    ...  EUR   "   "
    JAPdata = [0] #  "    "    " ...                    ...  JAP   "   "
    cur_year = table[0][6] # The current year we are counting for. Note that the table is still sorted by year

    for row in table:
        if row[7] == 'NA' or row[6] == 'NA': # The data must exist or we cannot use this row
            continue

        if row[6] != cur_year: # Check if we are ina  new year
            USAdata.append(0) # Add a new element to increment
            EURdata.append(0) #   ...for each dataset
            JAPdata.append(0)

            years.append(cur_year) # Add the old year as a label
            cur_year = row[6] # Update to the new current year

        if row[7] == '1': # Then increment the USA count
            USAdata[len(years)] += 1
            continue
        if row[7] == '2': # Then increment the EUR count
            EURdata[len(years)] += 1
            continue
        if row[7] == '3': # Then increment the JAP count
            JAPdata[len(years)] += 1
    years.append(cur_year) # Add the final year as a label
    
    fig = plt.figure() # Reset the figure
    plt.suptitle('Origin of Cars by Year', fontsize=12, fontweight='bold') # Title for the graph we are making
    ax = fig.add_subplot(111) # We are plotting multiple things
    
    xdist = plt.arange(len(years)) # Distance between the xticks
    width = 0.3 # The width of each bar per xtick. We have threem so there will be whitespace between the groupings

    USAbars = ax.bar(xdist, USAdata, width, color = 'blue') # Blue USA bars
    EURbars = ax.bar(xdist + width, EURdata, width, color = 'green') # Green EUR bars
    JAPbars = ax.bar(xdist + width * 2, JAPdata, 0.3, color = 'red') # Red JAP bars

    ax.set_xticks(xdist + width * 1.5)
    labels = ['19' + year for year in years] # '19 in front of every label for a four digit year
    ticklabels = ax.set_xticklabels(labels) # Set the tick labels
    plt.setp(ticklabels, rotation=45) # Rotate the labels a little for readability

    ax.legend((USAbars[0],EURbars[0],JAPbars[0]), ('USA','EUR','JAP')) # Create the legend
    plt.xlabel('Model Year') # Set x axis label
    plt.ylabel('Number of Cars Produced') # Set y axis label
    
    fig.savefig('step-8-chart2.pdf') # Save the figure
    if show_graphs: # Show the graph if preferred
        plt.show()
    plt.close() # Clean up

""" The main function """
def main():
    global show_graphs
    show_graphs = True
    table = read_csv('auto-data.txt') # Read in the automotive data
    #step1(table) # Perform step 1
    #step5(table) # Perform step 5
    step8(table) # Perform step 8

""" Entry point """
if __name__ == '__main__':
    main()
