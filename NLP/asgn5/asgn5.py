
"""
Makes a frequency chart and list of ngrams.
"""
def make_ngrams(sentences, n):
        grams = {}
        for sentence in sentences:
                gram = {}
                

        return grams

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
	sentences = getSentences()
	unigrams = make_ngrams(sentences, 1)

if __name__ == '__main__':
	main()

