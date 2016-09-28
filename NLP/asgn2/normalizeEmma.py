def normalize(inFile):
	import re

	strOut = open(inFile).read() # Read in the file

	pat = re.compile("[^A-Za-z ]") # Everything that isn't a letter or space
	strOut = pat.sub("", strOut) # Remove

	pat = re.compile("[\r\t\n\f]| +") # Every kind of whitespace or multiple spaces
	strOut = pat.sub(" ", strOut) # Be a single whitespace

	arr = strOut.lower().split() # Lowercase, tokenize, then fill array
	arr.sort() # Sort the array

	table = [] # Stores all the tuples
	curstr = arr[0] # Tracks the current string to count
	count = 0	# Counts the number of instances of curstr

	for element in arr: # Make unique and count
		if curstr == element: # Still counting the same word
			count = count + 1
		else: # New word
			table.append([count,curstr]) # Save the old one
			count = 1 # Reset the count
			curstr = element # Set the new word

	table.sort(reverse=True) # Sort in descending order

	return table
	
def writeToFile(data, outFile):
	total = 0
	for num,word in data: # Count the total number of words
		total = total + num

	out = open(outFile, 'w') # Prepare the file to write to
	out.write(str(total) + ' words\n') # Write the total number of words
	out.write(str(len(data)) + ' unique words\n') # Write the total number of words
	
	for num,word in data: # Write the number of times each word was used, then the word
		out.write(str(num) + ' ' + word + '\n')
		
def plotData(data):
	import matplotlib.pylab as np
	import math
	
	xs = [] # The x values for both graphs
	zys = [] # The y values for the Zipf graph
	eys = [] # The y values for the Emma graph
	for i in range(len(data)): # A value for every word
		xs.append(math.log(i+1)) # Take the log of the rank
		eys.append(math.log(data[i][0])) # Take the log of the frequency
		zys.append(-xs[i] + eys[0]) # Mathematically equivalent to log(1/rank)+intercept_of_eys

	np.plot(xs, zys, label="Natural Zipfian Distribution") # Plot the Zipf Graph
	np.plot(xs, eys, label="Emma's Zipfian Distribution") # Plot the Emma Graph

	
	np.show() # Show the graph

if __name__ == "__main__":
	data = normalize('austen-emma.txt') # Normalize the data
	writeToFile(data, 'out_emma.txt') # Write to file
	plotData(data) # Plot it
