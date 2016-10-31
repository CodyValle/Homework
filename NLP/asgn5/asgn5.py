"""
Cody Valle and Jinous Esmaeili
Natural Language Processing Assignment 5: N-Grams of the Brown Corpus
"""

import decimal
from decimal import Decimal
import random

"""
Returns a random number between 0(inclusive) and 1(exclusive) with currently set decimal precision
Code from: http://stackoverflow.com/questions/31595594/python-random-float-with-limited-decimal-digits
"""
def random_float():
        return Decimal(round(random.uniform(0, 1), decimal.getcontext().prec))

"""
Chooses a random N-Gram based on frequency
"""
def random_ngram(grams):
        # Get a random number between 0 and 1, with desired precision
        percent = random_float()
        for key,val in grams.iteritems():
                # Count down the probability until less than zero
                percent -= val
                if percent < 0:
                        return key
        return None # Should never run

"""
Makes an N-Gram sentence that starts with '<s>' and ends with '</s>'
"""
def ngram_sentence(grams, starts):
        ret = ''
        # Get the start of the sentence
        choice = random.choice(starts)

        # Append the gram to the return string
        for word in choice:
                ret += word + ' '

        # Keep choosing until we get an end of sentence
        choice = ['']
        while choice[-1] != '</s>':
                choice = random_ngram(grams)
                # Append the gram to the return string
                for word in choice:
                        ret += word + ' '

        return ret

"""
Makes a list of N-Grams from the passed in words
"""
def make_ngrams(words, n):
        grams = {}

        # Create a gram starting at every word
        for index in range(len(words) - n):
                # Create the n-length gram
                gram_list = []
                for offset in range(n):
                        gram_list.append(words[index + offset])
                #Convert to tuple to make it hashable
                gram_tuple = tuple(gram_list)
                grams[gram_tuple] = grams.get(gram_tuple,0) + 1 # Add and/or increment

        # Calculate the probabilities
        count = Decimal(len(grams))
        for key,val in grams.iteritems():
                grams[key] = Decimal(val) / count

        # Get the starts of sentences
        starts = []
        for key,_ in grams.iteritems():
                if key[0] == '<s>':
                        starts.append(key)
                
        return grams, starts

"""
Flattens a 2-dimensional array into a 1-dimensional array
"""
def flatten(table):
        ret = []
        for row in table:
                ret += [element for element in row]
        return ret

"""
Gets the cleaned sentences from the Brown corpus.
"""
def getSentences():
        import re
        import nltk
        from nltk.corpus import brown

        # Get the Brown corpus
        sentences_u = brown.sents(categories='editorial')
        sentences = [] # Will hold the list of cleaned sentences
        pat = re.compile('[A-Za-z]') # Matches strings with only letters

        for sentence_u in sentences_u: # Clean every sentence
                sentence = ['<s>'] # The start of every sentence
                for word_u in sentence_u:
                        word = word_u.encode('ascii','ignore') # Encode to ASCII
                        if pat.match(word): # Make sure it's just a word
                                sentence.append(word)

                # We have t make sure we have words in this sentence
                if sentence[-1] != '<s>':
                        sentences.append(sentence + ['</s>'])

        return sentences 

"""
The main function.
"""
def main():
        decimal.getcontext().prec = 14 # Double the default precision
        
        # Get the sentences from the Brown corpus
        sentences = getSentences()
        words = flatten(sentences) # Flatten them into a single list
        
        # Get the N-Grams
        unigrams, unistarts = make_ngrams(words, 1)
        bigrams, bistarts = make_ngrams(words, 2)
        trigrams, tristarts = make_ngrams(words, 3)
        quadgrams, uquadstarts = make_ngrams(words, 4)

        # Make N-Gram sentences
        for i in range(3):
                print str(i) + ':', ngram_sentence(trigrams, tristarts)

if __name__ == '__main__':
    main()

