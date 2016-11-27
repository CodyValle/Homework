"""
Cody Valle and Jinous Esmaeili
Natural Language Processing Assignment 6: POS Tagging based off of the Brown Corpus
To use:
Ensure NLTK is installed, and makes ure the Brown corpus is available.
Run the script using Python2.7.
Pass in a sentence as arguments, e.g python asgn6_test.py The grand jury took a swipe at the State Welfare "Department's" handling of federal funds granted for child welfare services in foster homes .
"""

import decimal
from decimal import Decimal

def viterbi(toParse, a, b, words, tags):
    ret = []

    """ Notes:
        a['first']['second'] is the odds of second following first
        b['tag']['word'] is the odds of 'tag' belonging to 'word'
    """

    """ Calculate the odds of each tag given the first word """
    try:
        aProbs = Decimal(a[words['.']][words[toParse[0]]])
    except ValueError:
        print 'No data for that as a start word.'
        return

    bestOdds = Decimal(0)
    bestTag = ''
    for tag,tagIndex in tags.iteritems():
        try:
            bProbs = Decimal(b[tagIndex][words[toParse[0]]])
        except ValueError:
            bProbs = Decimal(0)

        test = aProbs * bProbs
        if test > bestOdds:
            bestTag = tag
    ret.append(bestTag)

    """ For every following word, calculate the odds of going to each tag from every previous tag """
    for t in range(1, len(toParse)):
        prevWordIndex = words[toParse[t - 1]]
        curWordIndex = words[toParse[t]]
        try:
            aProbs = Decimal(a[prevWordIndex][curWordIndex]) # Odds of going from previous word to this word
        except ValueError:
            aProbs = Decimal(0)
        if aProbs == Decimal(0):
            continue
        
        bestTag = ''
        vmx = Decimal(0)
        for tag,tagIndex in tags.iteritems():
            try:
                bProbs = Decimal(b[tagIndex][curWordIndex])
            except ValueError:
                bProbs = Decimal(0)
            test = aProbs * bProbs
            if test > vmx:
                vmx = test
                bestTag = tag
        ret.append(bestTag)

    return ret

""" The main function """
def main():
        import pickle
        import sys
        import random
        decimal.getcontext().prec = 56 # Double the default precision is 56

        toParse = sys.argv[1:]
        
        print 'Parsing:', toParse
        with open('b.pickle', 'rb') as handle:
                b = pickle.load(handle)
        with open('a.pickle', 'rb') as handle:
                a = pickle.load(handle)
        with open('tags.pickle', 'rb') as handle:
                tags = pickle.load(handle)
        with open('words.pickle', 'rb') as handle:
                words = pickle.load(handle)

        ret = viterbi(toParse, a, b, words, tags)

        for i in range(len(ret)):
            print (toParse[i], ret[i])

if __name__ == '__main__':
    main()
