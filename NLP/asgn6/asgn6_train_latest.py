"""
Cody Valle and Jinous Esmaeili
Natural Language Processing Assignment 6: POS Tagging based off of the Brown Corpus
To use:
Ensure NLTK is installed, and makes ure the Brown corpus is available.
Run the script using Python2.7.
"""

import decimal
from decimal import Decimal       

""" Creates the A and B matrices """
def create_a_and_b(tagged):
        # The counts of each tag, each tag followed by a certain tag, and each word tagged with a tag
        tag_count, word_count, tag_tag_count, tag_word_count = {}, {}, {}, {}

        """
        NOTES:
                tag_count[tag] is the number of times tag appeared
                tag_tag_count[tag1][tag2] is the number of times tag2 was followed by tag1
                tag_word_count[tag][word] is the number of times word was tagged with tag
        """

        # Initialize
        pair = tagged[0]
        tag_count[pair[1]] = 1 # Update tag count
        word_count[pair[0]] = 1 # Update word count
        tag_word = {} 
        tag_word[pair[1]] = 1
        tag_word_count[pair[0]] = tag_word # Update tag word count
        prev = pair
        
        # Collect the data
        for pair in tagged[1:]:
                # Update tag and word count
                tag_count[pair[1]] = tag_count.get(pair[1], 0) + 1
                word_count[pair[0]] = word_count.get(pair[0], 0) + 1 # Update word count

                # Update tag tag count
                tag_tag = tag_tag_count.get(prev[1], {})
                tag_tag[pair[1]] = tag_tag.get(pair[1], 0) + 1
                tag_tag_count[prev[1]] = tag_tag

                # Update tag word count
                tag_word = tag_word_count.get(pair[1], {})
                tag_word[pair[0]] = tag_word.get(pair[0], 0) + 1
                tag_word_count[pair[1]] = tag_word

                # Prepare for next iteration
                prev = pair

        # Create the a matrix
        """
        NOTES:
              a[tag1][tag2] is the probability of tag2 following tag1  
        """
        a = {coltag: {rowtag: Decimal(tag_tag_count.get(coltag, {}).get(rowtag, 0)) / count for rowtag in tag_count.keys()} for coltag, count in tag_count.iteritems()}
                
        # Create the b matrix
        """
        NOTES:
                b[tag][word] is the probability of word being tagged tag
        """
        b = {tag: {word: Decimal(tag_word_count.get(tag, {}).get(word, 0)) / count for word in word_count.keys()} for tag, count in tag_count.iteritems()}

        return a, b

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
        # Convert the words to ASCII
        tagged = [(tup[0].encode('ascii'), tup[1].encode('ascii')) for tup in brown_tag]
        # Check proper length
        #print 'Proper length:', len(words) == 1161192
        # Clean the dataset, remove everything that isn't a word or letter
        letters = re.compile('[a-z]') # Matches strings with only letters
        notletters = re.compile('[^A-Za-z]') # Matches on anything that isn't a letter
        clean = []
        for pair in tagged:
                word = notletters.sub('', pair[0]) # Remove everything that isn't a letter
                word = word.lower() # make all the letters lowercase
                tag = pair[1]
                if letters.match(word):
                        clean.append((word, tag))

        return clean      

""" The main function """
def main():
        import pickle
        decimal.getcontext().prec = 56 # Double the default precision

        # Get the sentences from the Brown corpus
        tagged = get_words()[:100000]
        #for word in tagged[:500]:
        #       print word
        #return

        # Create training and test sets
        #brown_train, brown_test = create_test_and_train(tagged)

        # Create A nd B matrix
        #prof = cProfile.Profile() # Create the profiler
        #prof.enable() # Start the profiler
        a, b = create_a_and_b(tagged)
        #prof.disable() # Stop the profiler
        #stats = pstats.Stats(prof).sort_stats('cumtime') # Sort by time in function
        #stats.strip_dirs()
        #stats.print_stats('asgn6') # Restrict stats to functions called in this file

        print 'Done creating the matrices'
        with open('a.pickle', 'wb') as handle:
                pickle.dump(a, handle)
        print 'Done writing the a matrix'
        with open('b.pickle', 'wb') as handle:
                pickle.dump(b, handle)
        print 'Done writing the b matrix'

if __name__ == '__main__':
    main()
