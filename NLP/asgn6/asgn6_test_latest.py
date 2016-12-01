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

def viterbi(toParse, a, b):
    # Create the viterbi matrix and backpointer matrix
    v_matrix = {tag: {word: 0 for word in toParse} for tag in (a.keys())}
    backpointer = {tag: {word: 0 for word in toParse} for tag in a.keys()}

    """ Notes:
        a[tag1][tag2] is the probability of tag2 following tag1
        b[tag][word] is the probability of word being tagged tag
        v_matrix[tag][word] is the probability of being at that word with that tag
        backpointer[tag][word] is the most likely (tag,word) pair that led to this tag word pair
    """

    """ Calculate the odds of each tag given the first word """
    for tag in a.keys():
        prob = b[tag][toParse[0]]
        v_matrix[tag][toParse[0]] = prob
        backpointer[tag][toParse[0]] = (None, tag)

    """ Calculate the odds of the rest of the words """
    prevWord = toParse[0]
    for curword in toParse[1:]:
        for curtag in a.keys():
            # Calculate the most likely previous tag
            bProb = b[curtag][curword]
            bestTag = a.keys()[0]
            bestProb = 0
            for prevtag in a.keys():
                vProb = v_matrix[prevtag][prevWord]
                aProb = a[prevtag][curtag]
                testProb = vProb * aProb * bProb
                if testProb > bestProb:
                    bestProb = testProb
                    bestTag = prevtag
            # Update the v_matrix and backpointer
            v_matrix[curtag][curword] = bestProb
            backpointer[curtag][curword] = (bestTag, curtag)
        prevWord = curword

    # Calculate the final state
    bestProb = 0
    back = (a.keys()[0], None)
    for tag in a.keys():
        prob = v_matrix[tag][toParse[-1]]
        if prob > bestProb:
            bestProb = prob
            back = backpointer[tag][toParse[-1]]

    ret = []
    for word in toParse[-2::-1]:
        ret = [back[1]] + ret
        back = backpointer[back[0]][word]
    ret = [back[0]] + ret # Add in the 

    return ret

""" The main function """
def main():
        import pickle, sys, re
        decimal.getcontext().prec = 56 # Double the default precision is 56

        if len(sys.argv) < 2:
            print 'Error! Need at least one word to tag.'
            #return

        notletters = re.compile('[^A-Za-z]') # Matches on anything that isn't a letter
        toParse = [notletters.sub('',word).lower() for word in sys.argv[1:]]

        print 'Loading the a matrix...'
        with open('a.pickle', 'rb') as handle:
                a = pickle.load(handle)
        print 'Loading the b matrix...'
        with open('b.pickle', 'rb') as handle:
                b = pickle.load(handle)

        test1 = ['the','grand','jury','took','a','swipe','at','the','state','welfare','departments','handling']
        test1 += ['of','federal','funds','granted','for','child','welfare','services','in','foster','homes']
        """
        ('the', 'AT')
        ('grand', 'JJ')
        ('jury', 'NN')
        ('took', 'VBD')
        ('a', 'AT')
        ('swipe', 'NN')
        ('at', 'IN')
        ('the', 'AT')
        ('state', 'NN-TL')
        ('welfare', 'NN-TL')
        ('departments', 'NN$-TL')
        ('handling', 'NN')
        ('of', 'IN')
        ('federal', 'JJ')
        ('funds', 'NNS')
        ('granted', 'VBN')
        ('for', 'IN')
        ('child', 'NN')
        ('welfare', 'NN')
        ('services', 'NNS')
        ('in', 'IN')
        ('foster', 'JJ')
        ('homes', 'NNS')
        """
        print 'Parsing:', test1
        ret = viterbi(test1, a, b)

        print 'Results:'
        for i in range(len(ret)):
            print test1[i], ret[i]
            
        test2 = ['the','fulton','county','grand','jury','said','friday','an','investigation','of','atlantas']
        test2 += ['recent','primary','election','produced','no','evidence','that','any','irregularities','took']
        test2 += ['place','the','jury','further','said','in','termend','presentments','that', 'the','city']
        test2 += ['executive','committee','which','had','overall','charge','of','the','election','deserves']
        """
        ('the', 'AT')
        ('fulton', 'NP-TL')
        ('county', 'NN-TL')
        ('grand', 'JJ-TL')
        ('jury', 'NN-TL')
        ('said', 'VBD')
        ('friday', 'NR')
        ('an', 'AT')
        ('investigation', 'NN')
        ('of', 'IN')
        ('atlantas', 'NP$')
        ('recent', 'JJ')
        ('primary', 'NN')
        ('election', 'NN')
        ('produced', 'VBD')
        ('no', 'AT')
        ('evidence', 'NN')
        ('that', 'CS')
        ('any', 'DTI')
        ('irregularities', 'NNS')
        ('took', 'VBD')
        ('place', 'NN')
        ('the', 'AT')
        ('jury', 'NN')
        ('further', 'RBR')
        ('said', 'VBD')
        ('in', 'IN')
        ('termend', 'NN')
        ('presentments', 'NNS')
        ('that', 'CS')
        ('the', 'AT')
        ('city', 'NN-TL')
        ('executive', 'JJ-TL')
        ('committee', 'NN-TL')
        ('which', 'WDT')
        ('had', 'HVD')
        ('overall', 'JJ')
        ('charge', 'NN')
        ('of', 'IN')
        ('the', 'AT')
        ('election', 'NN')
        ('deserves', 'VBZ')
        """
        print 'Parsing:', test2
        ret = viterbi(test2, a, b)

        print 'Results:'
        for i in range(len(ret)):
            print test2[i], ret[i]
            
        test3 = ['it','can','be','said','that','the','techniques','and','standards','of']
        test3 +=['present','day']
        """
        ('it', 'PPS')
        ('can', 'MD')
        ('be', 'BE')
        ('said', 'VBN')
        ('that', 'CS')
        ('the', 'AT')
        ('techniques', 'NNS')
        ('and', 'CC')
        ('standards', 'NNS')
        ('of', 'IN')
        ('present', 'JJ')
        ('day', 'NN')
        """
        print 'Parsing:', test3
        ret = viterbi(test3, a, b)

        print 'Results:'
        for i in range(len(ret)):
            print test3[i], ret[i]

if __name__ == '__main__':
    main()
