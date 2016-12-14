import matplotlib.pylab as plt

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

""" Creates a plot of the locations of the buoys """
def buoy_location_plot(table, show = False):
    # We modulate longitude because coordinates wrap at -180 and 180
    coords = [(i[LONGITUDE] % 360, i[LATITUDE]) for i in table]

    xs = []
    ys = []
    for coord in coords:
        xs.append(coord[0])
        ys.append(coord[1])

    plt.figure() # Reset the figure
    plt.suptitle('Buoy Coordinates', fontsize=12, fontweight='bold')
    plt.grid(True) # Turn on the grid

    plt.scatter(xs, ys, c='g') # Plot the data

    # We have to override the actual numbers with normal data
    xlabels = [120, 140, 160, 0, -160, -140, -120, -100]
    plt.xticks(range(120, 270, 20), xlabels)
    plt.yticks(range(-10, 10, 2))
    
    plt.xlim(min(xs) - 5, max(xs) + 5) # Set the x limits
    plt.ylim(min(ys) - 0.2, max(ys) + 0.2) # Set the y limits
    
    plt.xlabel('Longitude')
    plt.ylabel('Latitude')

    if show:
        plt.show()
    plt.savefig('buoy_location.pdf') # Save the figure
    plt.close() # Clean up

""" Scatterplot of sea_temp versus month """
def sea_temp_vs_month_plot(table, show = False):
    coords = [(i[MONTH], i[SEA_TEMP]) for i in table]

    xs = []
    ys = []
    for coord in coords:
        xs.append(coord[0])
        ys.append(coord[1])

    plt.figure() # Reset the figure
    plt.suptitle('Sea Temperature vs. Month', fontsize=12, fontweight='bold')
    plt.grid(True) # Turn on the grid

    plt.scatter(xs, ys, c='b') # Plot the data

    # We have to override the actual numbers with normal data
    xlabels = ['January','February','March','April','May','June','July']
    xlabels += ['August','September','October','November','December']
    plt.xticks(range(1,13), xlabels,rotation=65)
    plt.yticks(range(int(min(ys)), int(max(ys))))
    
    plt.xlim(0, 13) # Set the x limits
    plt.ylim(min(ys) - 3, max(ys) + 3) # Set the y limits
    
    plt.xlabel('Month')
    plt.ylabel('Sea Temperature (C)')

    plt.gcf().subplots_adjust(bottom=0.20, top = 0.90)

    if show:
        plt.show()
    plt.savefig('sea_temp_vs_month.pdf') # Save the figure
    plt.close() # Clean up

""" Creates a scatter plot of sea temp vs year """
def sea_temp_vs_year_box(table, show = False):
    coords = [(i[YEAR], i[SEA_TEMP]) for i in table]

    xs = []
    ys = []
    for coord in coords:
        xs.append(coord[0])
        ys.append(coord[1])

    plt.figure() # Reset the figure
    plt.suptitle('Sea Temperature vs. Year', fontsize=12, fontweight='bold')
    plt.grid(True) # Turn on the grid

    plt.scatter(xs, ys, c='b') # Plot the data

    # We have to override the actual numbers with normal data
    xlabels = list(set(xs))
    xlabels.sort()
    xlabels = ['19' + str(x) for x in xlabels]
    plt.xticks(range(min(xs),max(xs) + 1),xlabels,rotation=40)
    plt.yticks(range(int(min(ys)), int(max(ys))))
    
    plt.xlim(min(xs) - 1, max(xs) + 1) # Set the x limits
    plt.ylim(min(ys) - 0.4, max(ys) + 0.4) # Set the y limits
    
    plt.xlabel('Year')
    plt.ylabel('Sea Temperature (C)')

    plt.gcf().subplots_adjust(bottom=0.16, top = 0.90)

    if show:
        plt.show()
    plt.savefig('sea_temp_vs_year.pdf') # Save the figure
    plt.close() # Clean up

""" Creates a scatter plot of sea temperature vs air temperature """
def sea_temp_vs_air_temp_box(table, show = False):
    coords = [(i[AIR_TEMP], i[SEA_TEMP]) for i in table]

    xs = []
    ys = []
    for coord in coords:
        xs.append(coord[0])
        ys.append(coord[1])

    plt.figure() # Reset the figure
    plt.suptitle('Sea Temperature vs. Air Temperature', fontsize=12, fontweight='bold')
    plt.grid(True) # Turn on the grid

    plt.scatter(xs, ys, c='b') # Plot the data

    # We have to override the actual numbers with normal data
    xlabels = list(set([int(x) for x in xs]))
    xlabels.sort()
    plt.xticks(range(int(min(xs)),int(max(xs)) + 1),xlabels)
    plt.yticks(range(int(min(ys)), int(max(ys))))
    
    plt.xlim(min(xs) - 0.4, max(xs) + 0.4) # Set the x limits
    plt.ylim(min(ys) - 0.4, max(ys) + 0.4) # Set the y limits
    
    plt.xlabel('Air Temperature (C)')
    plt.ylabel('Sea Temperature (C)')

    plt.gcf().subplots_adjust(bottom=0.10, top = 0.90)

    if show:
        plt.show()
    plt.savefig('sea_temp_vs_air_temp.pdf') # Save the figure
    plt.close() # Clean up

""" Creates a sinusoidal relationship """
def sinusoidal(table, show):
    # Code from http://stackoverflow.com/questions/16716302/how-do-i-fit-a-sine-curve-to-my-data-with-pylab-and-numpy
    import numpy as np
    from scipy.optimize import curve_fit

    coords = [(i[MONTH], i[SEA_TEMP]) for i in table]

    xs = []
    ys = []
    for coord in coords:
        xs.append(coord[0])
        ys.append(coord[1])

    guess_freq = 1
    guess_amplitude = 3*np.std(ys)/(2**0.5)
    guess_phase = 0
    guess_offset = np.mean(ys)

    p0=[guess_freq, guess_amplitude,
        guess_phase, guess_offset]

    # create the function we want to fit
    def my_sin(x, freq, amplitude, phase, offset):
        return np.sin(x * freq + phase) * amplitude + offset

    # now do the fit
    fit = curve_fit(my_sin, xs, ys, p0=p0)

    # we'll use this to plot our first estimate. This might already be good enough for you
    data_first_guess = my_sin(xs, *p0)

    # recreate the fitted curve using the optimized parameters
    data_fit = my_sin(xs, *fit[0])

    plt.plot(data, '.')
    plt.plot(data_fit, label='after fitting')
    plt.plot(data_first_guess, label='first guess')
    plt.legend()
    plt.show()

""" Creates various plots of the data """
def do_plots(table, show = False):
    #buoy_location_plot(table, show)
    #sea_temp_vs_month_plot(table, show)
    #sea_temp_vs_year_box(table, show)
    #sea_temp_vs_air_temp_box(table, show)
    sinusoidal(table, show)

""" To make this an executable """
if __name__ == '__main__':
    import proj
    proj.main()
