# Cody Valle
# Jinous Esmaeili
# GU ID: cvalle

import sys

def q0(character):
        return ('q1', character
                )

def q1(character):
        if character in 'AEIOUWHY':
                return 'q1', ''
        if character in 'BFPV':
                return 'q2', '1'
        if character in 'CGJKQSXZ':
                return 'q3', '2'
        if character in 'DT':
                return 'q4', '3'
        if character in 'L':
                return 'q5', '4'
        if character in 'MN':
                return 'q6', '5'
        if character in 'R':
                return 'q7', '6'

def q2(curstate, character):
        if character in 'AEIOUY':
                return 'q9', ''
        if character in 'HW':
                return curstate, ''
        if character in 'BFPV':
                return 'q2', ('1' if curstate != 'q2' else '')
        if character in 'CGJKQSXZ':
                return 'q3', ('2' if curstate != 'q3' else '')
        if character in 'DT':
                return 'q4', ('3' if curstate != 'q4' else '')
        if character in 'L':
                return 'q5', ('4' if curstate != 'q5' else '')
        if character in 'MN':
                return 'q6', ('5' if curstate != 'q6' else '')
        if character in 'R':
                return 'q7', ('6' if curstate != 'q7' else '')

def q3(character):
        if character in 'AEIOUWHY':
                return 'q9', ''
        if character in 'BFPV':
                return 'q10', '1'
        if character in 'CGJKQSXZ':
                return 'q11', '2'
        if character in 'DT':
                return 'q12', '3'
        if character in 'L':
                return 'q13', '4'
        if character in 'MN':
                return 'q14', '5'
        if character in 'R':
                return 'q15', '6'

def q4(curstate, character):
        if character in 'AEIOUY':
                return 'q9', ''
        if character in 'HW':
                return curstate, ''
        if character in 'BFPV':
                return 'q16', ('1' if curstate != 'q10' else '')
        if character in 'CGJKQSXZ':
                return 'q16', ('2' if curstate != 'q11' else '')
        if character in 'DT':
                return 'q16', ('3' if curstate != 'q12' else '')
        if character in 'L':
                return 'q16', ('4' if curstate != 'q13' else '')
        if character in 'MN':
                return 'q16', ('5' if curstate != 'q14' else '')
        if character in 'R':
                return 'q16', ('6' if curstate != 'q15' else '')

def q5(character):
      return 'q16', '0'  

def FST(toParse):
        print 'Parsing "' + toParse + '" with the FST.'

        toParse = toParse.upper()

        output = ''

        (state, character) = q0(toParse[0])
        output += character

        for c in toParse[1:]:
                if state == 'q1':
                       (state, character) = q1(c)
                elif state == 'q2':
                       (state, character) = q2(state, c)
                elif state == 'q3':
                       (state, character) = q2(state, c)
                elif state == 'q4':
                       (state, character) = q2(state, c)
                elif state == 'q5':
                       (state, character) = q2(state, c)
                elif state == 'q6':
                       (state, character) = q2(state, c)
                elif state == 'q7':
                       (state, character) = q2(state, c)
                elif state == 'q8':
                       (state, character) = q2(state, c)
                elif state == 'q9':
                       (state, character) = q3(c)
                elif state == 'q10':
                       (state, character) = q4(state, c)
                elif state == 'q11':
                       (state, character) = q4(state, c)
                elif state == 'q12':
                       (state, character) = q4(state, c)
                elif state == 'q13':
                       (state, character) = q4(state, c)
                elif state == 'q14':
                       (state, character) = q4(state, c)
                elif state == 'q15':
                       (state, character) = q4(state, c)
                elif state == 'q16':
                       (state, character) = q5(c)

                output += character

        output = output[:4]
        output = output + '0' * (4 - len(output))
        print 'FST Soundex output "' + output + '".'

def main(toParse):
	import re
	
	print 'Parsing "' + toParse + '" the old way.'

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

	print 'Old Soundex output "' + toParse + '".'

if __name__ == '__main__':
	if len(sys.argv) < 2:
		print 'Error! Need at least one argument to parse.'
	else:
		for i in range(1, len(sys.argv)):
			main(sys.argv[i])
			

	FST('Herman')
	main('Herman')
	FST('Cody')
	main('Cody')
	FST('Jinous')
	main('Jinous')
	FST('Kitty')
	main('Kitty')
	FST('bigger')
	main('bigger')
	FST('sassy')
	main('sassy')
	FST('ashcraft')
	main('ashcraft')
	FST('achsraft')
	main('achsraft')
	FST('Rubin')
	main('Rubin')
	FST('Rupert')
	main('Rupert')
