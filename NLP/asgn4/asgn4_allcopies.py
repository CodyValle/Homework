# Cody Valle
# Jinous Esmaeili
# GU ID: cvalle

"""
The states that exist in this transducer are simulated by the functions
q0, q1, q2, q3, q4, and q5. There are more states that exist in the actual
transducer, but many do the same thing, so they were lumped together.
"""

import sys

""" The start state of the Finite State Transducer.
        The parameter 'character' is the current character being read.
        Returns a tuple: (the next state to go to, what to print) """
def q0_old(character):
        return ('q1', character)

""" The second state of the Finite State Transducer.
        The parameter 'character' is the current character being read.
        Returns a tuple: (the next state to go to, what to print) """
def q1_old(character):
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

""" An intermediate state of the Finite State Transducer.
        The parameter 'character' is the current character being read.
        The parameter 'curstate' is the current state the transducer is in.
        Returns a tuple: (the next state to go to, what to print) """
def q2_old(curstate, character):
        if character in 'AEIOUY':
                # Consonant then vowel, possible repetition of printed number
                return 'q9', ''
        if character in 'HW':
                # Print nothing, stay in this state
                return curstate, ''
        if character in 'BFPV':
                # Don't print anything if we encounter a consonant from the same group
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

""" An intermediate state of the Finite State Transducer.
        The parameter 'character' is the current character being read.
        Returns a tuple: (the next state to go to, what to print) """
def q3_old(character):
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

""" An intermediate state of the Finite State Transducer.
        The parameter 'character' is the current character being read.
        The parameter 'curstate' is the current state the transducer is in.
        Returns a tuple: (the next state to go to, what to print) """
def q4_old(curstate, character):
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

""" The final state of the Finite State Transducer. Loops back to
        itself and prints zeroes until the word is done.
        The parameter 'character' is the current character being read.
        Returns a tuple: (the next state to go to, what to print) """
def q5_old(character):
      return 'q16', '0'

def FST_trim(state, input):
        if len(input) == 0:
                if state == 'q1':
                        return '0' + FST_trim('q0', input)
                if state == 'q2':
                        return '0' + FST_trim('q1', input)
                if state == 'q3':
                        return '0' + FST_trim('q2', input)
        if state == 'q0':
                return ''
        if state == 'q1':
                return input[0] + FST_trim('q0', input[1:])
        if state == 'q2':
                return input[0] + FST_trim('q1', input[1:])
        if state == 'q3':
                return input[0] + FST_trim('q2', input[1:])
        if state == 'q4':
                return input[0] + FST_trim('q3', input[1:])

""" Models a Finite State Transducer
        The parameter 'toParse' is the string to run through
        the transducer. """
def FST_old(toParse):
        print 'Parsing "' + toParse + '".'

        toParse = toParse.upper() # Step 1

        # The result of the FST
        output = ''

        # state is the state the actual FST would be in
        state = 'q0'

        for c in toParse:
                # This large if elif block maps the actual state to the function states
                if state == 'q0':
                       (state, character) = q0_old(c)
                elif state == 'q1':
                       (state, character) = q1_old(c)
                elif state == 'q2':
                       (state, character) = q2_old(state, c)
                elif state == 'q3':
                       (state, character) = q2_old(state, c)
                elif state == 'q4':
                       (state, character) = q2_old(state, c)
                elif state == 'q5':
                       (state, character) = q2_old(state, c)
                elif state == 'q6':
                       (state, character) = q2_old(state, c)
                elif state == 'q7':
                       (state, character) = q2_old(state, c)
                elif state == 'q8':
                       (state, character) = q2_old(state, c)
                elif state == 'q9':
                       (state, character) = q3_old(c)
                elif state == 'q10':
                       (state, character) = q4_old(state, c)
                elif state == 'q11':
                       (state, character) = q4_old(state, c)
                elif state == 'q12':
                       (state, character) = q4_old(state, c)
                elif state == 'q13':
                       (state, character) = q4_old(state, c)
                elif state == 'q14':
                       (state, character) = q4_old(state, c)
                elif state == 'q15':
                       (state, character) = q4_old(state, c)
                elif state == 'q16':
                       (state, character) = q5_old(c)

                output += character # Append the printed character

        output = FST_trim('q4', output)
        print 'Soundex output "' + output + '".'

""" The start state of the Finite State Transducer.
        The parameter 'character' is the current character being read.
        Returns a tuple: (the next state to go to, what to print) """
def function0(curstate, input):
        return input[0] + function1('q1', input[1:])

""" The second state of the Finite State Transducer.
        The parameter 'character' is the current character being read.
        Returns a tuple: (the next state to go to, what to print) """
def function1(curstate, input):
        if len(input) == 0:
                return '0' + function5('q18', input)
        if input[0] in 'AEIOUWHY':
                return function1('q1', input[1:])
        if input[0] in 'BFPV':
                return '1' + function2('q2', input[1:])
        if input[0] in 'CGJKQSXZ':
                return '2' + function2('q3', input[1:])
        if input[0] in 'DT':
                return '3' + function2('q4', input[1:])
        if input[0] in 'L':
                return '4' + function2('q5', input[1:])
        if input[0] in 'MN':
                return '5' + function2('q6', input[1:])
        if input[0] in 'R':
                return '6' + function2('q7', input[1:])

""" An intermediate state of the Finite State Transducer.
        The parameter 'character' is the current character being read.
        The parameter 'curstate' is the current state the transducer is in.
        Returns a tuple: (the next state to go to, what to print) """
def function2(curstate, input):
        if len(input) == 0:
                return '0' + function5('q17', input)
        if input[0] in 'AEIOUY':
                # Consonant then vowel, possible repetition of printed number
                return function3('q9', input[1:])
        if input[0] in 'HW':
                # Print nothing, stay in this state
                return function2(curstate, input[1:])
        if input[0] in 'BFPV':
                # Don't print anything if we encounter a consonant from the same group
                return ('1' if curstate != 'q2' else '') + function2('q2', input[1:])
        if input[0] in 'CGJKQSXZ':
                return ('2' if curstate != 'q3' else '') + function2('q3', input[1:])
        if input[0] in 'DT':
                return ('3' if curstate != 'q4' else '') + function2('q4', input[1:])
        if input[0] in 'L':
                return ('4' if curstate != 'q5' else '') + function2('q5', input[1:])
        if input[0] in 'MN':
                return ('5' if curstate != 'q6' else '') + function2('q6', input[1:])
        if input[0] in 'R':
                return ('6' if curstate != 'q7' else '') + function2('q7', input[1:])

""" An intermediate state of the Finite State Transducer.
        The parameter 'character' is the current character being read.
        Returns a tuple: (the next state to go to, what to print) """
def function3(curstate, input):
        if len(input) == 0:
                return '0' + function5('q17', input)
        if input[0] in 'AEIOUWHY':
                return function3('q9', input[1:])
        if input[0] in 'BFPV':
                return '1' + function6('q10', input[1:])
        if input[0] in 'CGJKQSXZ':
                return '2' + function6('q11', input[1:])
        if input[0] in 'DT':
                return '3' + function6('q12', input[1:])
        if input[0] in 'L':
                return '4' + function6('q13', input[1:])
        if input[0] in 'MN':
                return '5' + function6('q14', input[1:])
        if input[0] in 'R':
                return '6' + function6('q15', input[1:])

""" An intermediate state of the Finite State Transducer.
        The parameter 'character' is the current character being read.
        The parameter 'curstate' is the current state the transducer is in.
        Returns a tuple: (the next state to go to, what to print) """
def function4(curstate, input):
        if len(input) == 0:
                return '0'
        if input[0] in 'AEIOUY':
                return function4('q9', input[1:])
        if input[0] in 'HW':
                return function4(curstate, input[1:])
        if input[0] in 'BFPV':
                return ('1' if curstate != 'q10' else '')
        if input[0] in 'CGJKQSXZ':
                return ('2' if curstate != 'q11' else '')
        if input[0] in 'DT':
                return ('3' if curstate != 'q12' else '')
        if input[0] in 'L':
                return ('4' if curstate != 'q13' else '')
        if input[0] in 'MN':
                return ('5' if curstate != 'q14' else '')
        if input[0] in 'R':
                return ('6' if curstate != 'q15' else '')

""" The final state of the Finite State Transducer. Loops back to
        itself and prints zeroes until the word is done.
        The parameter 'character' is the current character being read.
        Returns a tuple: (the next state to go to, what to print) """
def function5(curstate, input):
        if curstate == 'q16':
                return ''
        if curstate == 'q17':
                return '0'
        if curstate == 'q18':
                return '0' + function5('q17', input)

""" An intermediate state of the Finite State Transducer.
        The parameter 'character' is the current character being read.
        The parameter 'curstate' is the current state the transducer is in.
        Returns a tuple: (the next state to go to, what to print) """
def function6(curstate, input):
        if len(input) == 0:
                return '0'
        if input[0] in 'AEIOUY':
                return function6('q9', input[1:])
        if input[0] in 'HW':
                return function6(curstate, input[1:])
        if input[0] in 'BFPV':
                return ('1' if curstate != 'q10' else '') + function5('q17' if curstate != 'q9' else 'q16',input[1:])
        if input[0] in 'CGJKQSXZ':
                return ('2' if curstate != 'q11' else '') + function5('q17' if curstate != 'q9' else 'q16', input[1:])
        if input[0] in 'DT':
                return ('3' if curstate != 'q12' else '') + function5('q17' if curstate != 'q9' else 'q16',input[1:])
        if input[0] in 'L':
                return ('4' if curstate != 'q13' else '') + function5('q17' if curstate != 'q9' else 'q16',input[1:])
        if input[0] in 'MN':
                return ('5' if curstate != 'q14' else '') + function5('q17' if curstate != 'q9' else 'q16',input[1:])
        if input[0] in 'R':
                return ('6' if curstate != 'q15' else '') + function5('q17' if curstate != 'q9' else 'q16',input[1:])

""" Models a Finite State Transducer
        The parameter 'toParse' is the string to run through
        the transducer. """
def FST(toParse):
        print 'Parsing "' + toParse + '".'

        toParse = toParse.upper() # Step 1

        # The result of the FST
        output = function0('q0', toParse)

        # Print the output
        print 'Soundex output "' + output + '".'


""" Original implementation of the algorithm. Unused and kept for posterity. """
def main(toParse):
	import re
	
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

"""
Entry point. Usage: python asgn4.py word [word word ...]
"""
if __name__ == '__main__':
	if len(sys.argv) < 2:
		print 'Error! Need at least one argument to parse.'
	else:
                # Parse all arguments with the FST
		for i in range(1, len(sys.argv)):
                        main(sys.argv[i])
			FST_old(sys.argv[i])
                        FST(sys.argv[i])
