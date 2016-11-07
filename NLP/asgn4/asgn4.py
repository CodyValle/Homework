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

"""
Pads a length less than four string to length of four and cuts a length
greater than four string to length four.
"""
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

"""
Entry point. Usage: python asgn4.py word [word word ...]
"""
if __name__ == '__main__':
	if len(sys.argv) < 2:
		print 'Error! Need at least one argument to parse.'
	else:
                # Parse all arguments with the FST
		for i in range(1, len(sys.argv)):
			FST_old(sys.argv[i])
