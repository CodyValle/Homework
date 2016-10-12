# Cody Valle
# Jinous Esmaeili
# GU ID: cvalle

import sys

def main():
	import re
	
	global toParse
	print 'Parsing "' + toParse + '".'

	""" Step 1 """
	toParse = toParse.upper() # Capitalize every character
	toParse = toParse[0] +  re.compile("[^A-Z]").sub('', toParse[1:]) # Remove everything that isn't a capital letter

	""" Step 2 is implied """
	""" Step 3 """
	toParse = toParse[0] +  re.compile("[AEIOUHWY]").sub('0', toParse[1:]) # Converts listed characters to '0'

	""" Step 4 """
	toParse = toParse[0] +  re.compile("[BFPV]").sub('1', toParse[1:])
	toParse = toParse[0] +  re.compile("[CGJKQSXZ]").sub('2', toParse[1:])
	toParse = toParse[0] +  re.compile("[DT]").sub('3', toParse[1:])
	toParse = toParse[0] +  re.compile("[L]").sub('4', toParse[1:])
	toParse = toParse[0] +  re.compile("[MN]").sub('5', toParse[1:])
	toParse = toParse[0] +  re.compile("[R]").sub('6', toParse[1:])

	""" Step 5 """
	for i in range(1,6):
		toParse = toParse[0] +  re.compile(str(i) + "{2,}").sub(str(i), toParse[1:])

	""" Step 6 """
	toParse = toParse[0] +  re.compile("0").sub('', toParse[1:])

	""" Step 7 """
	toParse = toParse[:4]
	toParse = toParse + '0' * (4 - len(toParse))

	print 'Soundex output "' + toParse + '".'

if __name__ == '__main__':
	if len(sys.argv) < 2:
		print 'Error! Need at least one argument to parse.'
	else:
		for i in range(1, len(sys.argv)):
			toParse = sys.argv[i]
			main()
