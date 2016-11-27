"""
Cody Valle and Jinous Esmaeili
Natural Language Processing Assignment 6: POS Tagging based off of the Brown Corpus
To use:
Ensure NLTK is installed, and makes ure the Brown corpus is available.
Run the script using Python2.7.
"""

import decimal
from decimal import Decimal

""" Makes a list of N-Grams from the passed in words """
def make_bigrams(words):
	grams = {}
	num_words = len(words)

	# Create a gram starting at every word
	for index in range(num_words - 1):
		tup = (words[index][0], words[index + 1][0]) # Create the bigram
		grams[tup] = grams.get(tup, 0) + 1 # Add and/or increment

	return grams

""" Creates a set of second element that start with first """
def first_element(start, items):
        ret = {}
        for item in items:
                if item[0] == start:
                        ret[item[1]] = ret.get(item[1], 0) + 1
        return ret 

""" Creates a set of first element that end with second """
def second_element(second, items):
        ret = {}
        for item in items:
                if item[1] == second:
                        ret[item[0]] = ret.get(item[0], 0) + 1
        return ret        

""" Creates the A and B matrices """
def create_a_and_b(tagged):
        import pickle
        
        # The IDs and counts of each word, tag, and pair
        pairs, words, tags = {}, {}, {}
        tag_count, word_count = {}, {}
        num_pairs, num_words, num_tags = 0, 0, 0
        for pair in tagged:
                #if pair not in pairs.keys():
                        #pairs[pair] = num_pairs
                        #num_pairs += 1
                if pair[0] not in words.keys():
                        words[pair[0]] = num_words
                        num_words += 1
                if pair[1] not in tags.keys():
                        tags[pair[1]] = num_tags
                        num_tags += 1
                word_count[pair[0]] = word_count.get(pair[0], 0) + 1
                tag_count[pair[1]] = tag_count.get(pair[1], 0) + 1

        print 'Finished creating prep work!'
        with open('pairs.pickle', 'wb') as handle:
                pickle.dump(pairs, handle)
        with open('words.pickle', 'wb') as handle:
                pickle.dump(words, handle)
        with open('tags.pickle', 'wb') as handle:
                pickle.dump(tags, handle)
        print 'Finished writing prep work!'

        # All word/tag pair bigrams in this corpus
        bigrams = make_bigrams(tagged)
        num_bigrams = len(bigrams.keys())
        print 'Finished creating bigrams!'

        # Create the A matrix
        a = [[[0] for _ in range(num_pairs)] for _ in range(num_pairs)]

        # Fill the A matrix
        for word, x in words.iteritems():
                follows_words = first_element(word, bigrams)
                total = word_count[word]
                for follows, count in follows_words.iteritems():
                        a[x][words[follows]] = Decimal(count) / total
                        
        print 'Finished creating A matrix!'
        with open('a.pickle', 'wb') as handle:
                pickle.dump(a, handle)
        print 'Finished writing A matrix!'

        # Create the B matrix
        b = [[[0] for _ in range(num_words)] for _ in range(num_tags)]

        # Fill the B matrix
        for tag, x in tags.iteritems():
                word_tags = second_element(tag, tagged)
                total = tag_count[tag]
                for word, count in word_tags.iteritems():
                        b[x][words[word]] = Decimal(count) / total

        print 'Finished creating B matrix!'
        with open('b.pickle', 'wb') as handle:
                pickle.dump(b, handle)
        print 'Finished writing B matrix!'

        return a, b, pairs, words, tags

""" Creates a testing set and training set from a list of words """
def create_test_and_train(words):
        import random
        training_set = []
        test_set = []
        k = 0
        for row in words:
                if random.randint(0, 9) == 0: # 10% in the test set
                        test_set.append(row)
                else:
                        training_set.append(row)
                k += 1
        return training_set, test_set

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
        decimal.getcontext().prec = 56 # Double the default precision

        # Get the sentences from the Brown corpus
        words = get_words()

        # Create training and test sets
        brown_train, brown_test = create_test_and_train(words)

        # Create A nd B matrix
        a, b, pairs, words, tags = create_a_and_b(brown_train)

        print 'Finished everything!'

if __name__ == '__main__':
    main()
