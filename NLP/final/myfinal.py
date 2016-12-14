
def LHSCheck(rules, setRules):
    for rule in rules:
        setRules.append(rule[0])
    return set(setRules)

def RHSCheck(rules, LHSSet, RHSSet):
    for rule in rules:
        for tokens in rule[1]:
            if tokens not in LHSSet:
                RHSSet.append(tokens)
    return set(RHSSet)

# Gets the rules from the grammar
def getRules():
    rules = []
    with open('grammar.txt', 'r') as f:
        for line in f:
            s = line.split('->')
            RHSs = s[1].strip().split('|')
            for r in RHSs:
                rules.append((s[0].strip(), r.strip().split()))
    return rules

# Run the program
def main():
    from tabulate import tabulate
    rules = getRules()
    for r in rules:
        print r

    LHSSet = []
    RHSSet = []
    setRules = []
    LHSSet = LHSCheck(rules,setRules)
    RHSSet = RHSCheck(rules, LHSSet, RHSSet)

    # Parse the string
    tokens = []
    with open('string.txt', 'r') as f:
        for line in f:
            for word in line.split():
                tokens.append(word)

    # Create the matrix
    matrix = [[[] for _ in range(len(tokens))] for _ in range(len(tokens))]
    # Fill in the diagonal
    for i in range(len(tokens)):
        word = tokens[i]
        cell = []
        for r in rules:
            if word in r[1]:
                cell.append(r[0])
        matrix[i][i] = cell

    # Fill in the second diagonal
    for i in range(1, len(tokens)):
        first = matrix[i-1][i-1]
        second = matrix[i][i]
        cell = []
        for r in rules:
            if len(r[1]) > 1 and r[1][0] in first:
                if r[1][1] in second:
                    cell.append(r[0])
        matrix[i-1][i] = cell

    # Fill in the third diagonal
    for i in range(2, len(tokens)):
        i2 = i - 1
        first = []
        while i2 >= 0:
            if len(matrix[i-2][i2]) > 0:
                first += matrix[i-2][i2]
            i2 -= 1
        second = matrix[i-1][i]
        cell = []
        for r in rules:
            if len(r[1]) > 1 and r[1][0] in first:
                if r[1][1] in second:
                    cell.append(r[0])
        matrix[i-2][i] = cell

    # Fill in the fourth diagonal
    for i in range(3, len(tokens)):
        i2 = i - 1
        first = []
        while i2 >= 0:
            if len(matrix[i-3][i2]) > 0:
                first += matrix[i-3][i2]
            i2 -= 1
        second = matrix[i-2][i]
        cell = []
        for r in rules:
            if len(r[1]) > 1 and r[1][0] in first:
                if r[1][1] in second:
                    cell.append(r[0])
        matrix[i-3][i] = cell

    # Fill in the last cell
    i2 = 3
    first = []
    while i2 >= 0:
        if len(matrix[0][i2]) > 0:
            first += matrix[0][i2]
        i2 -= 1
    second = matrix[1][4]
    cell = []
    for r in rules:
        if len(r[1]) > 1 and r[1][0] in first:
            if r[1][1] in second:
                cell.append(r[0])
    matrix[0][4] = cell

    print tabulate(matrix)
    print 'Yes' if 'S' in matrix[0][4] else 'No'

if __name__ == '__main__':
    main()
