########################################################
# CYK Parser, NLP Final Fall 2016
#   by Cody Valle, Jinous Esmaeili, Sebastian Vargas
#   ---------------------------------------------------
#   This program uses the CYK parser to check if a
#   string is defined in a grammar

#######################################################
#   getRules()
#       reads a text file and returns a list of tuples
#       where the first item is the rule before the ->
#       and the second is after.
def getRules():
    with open('grammar.txt', 'r') as f:
        rules = []
        for line in f:
            s = line.split('->')
            rules += [(s[0].strip(), r.strip().split()) for r in s[1].strip().split('|')]
    return rules

#######################################################
#   getTokens()
#       reads a text file and returns the string
#       to be tested on as an array of words
def getTokens():
    # Parse the string
    with open('string.txt', 'r') as f:
        return [word for line in f for word in line.split()]

########################################################
#   CYKMatrix(tokens, rules)
#       creates the CYK matrix
def CYKMatrix(tokens, rules):
    # Create the matrix
    matrix = [[[] for _ in range(len(tokens))] for _ in range(len(tokens))]
    # Fill in the first diagonal
    for i in range(len(tokens)):
        matrix[i][i] = [r[0] for r in rules if tokens[i] in r[1]]

    # Fills in every other diagonal until the last    
    for j in range(1, len(tokens)):
        for i in range(j, len(tokens)):
            first = set([x for k in range(i-j+1) for x in matrix[i-j][k]]) # All first rules
            matrix[i-j][i] = [r[0] for r in rules
                              if r[1][0] in first # Check first rule exists
                              and r[1][1] in matrix[i-j+1][i] # Check second rule
                             ]
    return matrix

# Run the program
def main():
    # Get the rules
    rules = getRules()

    # Gets a set of all non-terminals
    LHSSet = set([rule[0] for rule in rules])
    # Get a set of all terminals
    RHSSet = set([token for rule in rules for token in rule[1] if token not in LHSSet])

    # Gets the string to parse
    tokens = getTokens()
    # Check that each token is a terminal of this language
    for token in tokens:
        if token not in RHSSet:
            print 'No'
            return

    # Create the matrix
    matrix = CYKMatrix(tokens, rules)
    
    print 'Yes' if 'S' in matrix[0][len(tokens)-1] else 'No'

    # Uncomment these lines to see the matrix
    from tabulate import tabulate
    print tabulate(matrix, headers = tokens)

if __name__ == '__main__':
    main()
