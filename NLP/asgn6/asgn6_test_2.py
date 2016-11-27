"""
Cody Valle and Jinous Esmaeili
Natural Language Processing Assignment 6: POS Tagging based off of the Brown Corpus
To use:
Ensure NLTK is installed, and makes ure the Brown corpus is available.
Run the asgn6_train.py script using Python2.7. It will take quite some time.
When it is completed, use the asgn6_test.py script to tag a passed in list of words.
"""

import decimal
from decimal import Decimal

""" Gets the cleaned tagged words from the Brown corpus """
def get_words():
	import re
	import nltk
	from nltk.corpus import brown

	# Get the Brown corpus
	brown_tag = brown.tagged_words()
	words = [(tup[0].encode('ascii'), tup[1].encode('ascii')) for tup in brown_tag]
	#print 'Proper length:', len(words) == 1161192

	return words       

""" The main function """
def main():
    import pickle
    import sys
    
    if len(sys.argv) < 2:
        print 'Error! Need at least one word to tag.'
        return

    decimal.getcontext().prec = 56 # Double the default precision
    
    toParse = sys.argv[1:]

    # Load the matrices
    with open('b.pickle', 'rb') as handle:
        b = pickle.load(handle)
    with open('a.pickle', 'rb') as handle:
        a = pickle.load(handle)
    with open('tags.pickle', 'rb') as handle:
        tags = pickle.load(handle)
    with open('words.pickle', 'rb') as handle:
        words = pickle.load(handle)

    

    for i in range(len(toParse) - 1):
        print a[words[toParse[i]]][words[toParse[i + 1]]]
    
    #print b[tags['IN']][words[toParse[i]]]

if __name__ == '__main__':
    main()
