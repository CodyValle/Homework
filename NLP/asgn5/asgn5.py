



def main():
	import re
	import nltk
	from nltk.corpus import brown
	sentences_u = brown.sents(categories='editorial')
	sentences = []
	pat = re.compile('[A-Za-z]')
	for sentence_u in sentences_u:
		sentence = ['<s>']
		for word_u in sentence_u:
			word = word_u.encode('ascii','ignore')
			if pat.match(word):
				sentence.append(word.encode('ascii','ignore'))
		sentences.append(sentence + ['</s>'])

	print sentences[:10]

if __name__ == '__main__':
	main()

