"""
Cody Valle, Andrew Burke, Katreina Carpenter
Data Mining Homework 7 Program
"""
import sets

# Globals for titanic data
global CLASS
CLASS = 0
global AGE
AGE = 1
global SEX
SEX = 2
global SURVIVED
SURVIVED = 3

# Globals for mushroom data
global CLASS_LABEL
CLASS_LABEL = 0
global CAP_SHAPE
CAP_SHAPE = 1
global CAP_SURFACE
CAP_SURFACE = 2
global CAP_COLOR
CAP_COLOR = 3
global BRUISES
BRUISES = 4
global ODOR
ODOR = 5
global GILL_ATTACHMENT
GILL_ATTACHMENT = 6
global GILL_SPACING
GILL_SPACING = 7
global GILL_SIZE
GILL_SIZE = 8
global GILL_COLOR
GILL_COLOR = 9
global STALK_SHAPE
STALK_SHAPE = 10
global STALK_ROOT
STALK_ROOT = 11
global STALK_SURFACE_ABOVE_RING
STALK_SURFACE_ABOVE_RING = 12
global STALK_SURFACE_BELOW_RING
STALK_SURFACE_BELOW_RING = 13
global STALK_COLOR_ABOVE_RING
STALK_COLOR_ABOVE_RING = 14
global STALK_COLOR_BELOW_RING
STALK_COLOR_BELOW_RING = 15
global VEIL_TYPE
VEIL_TYPE = 16
global VEIL_COLOR
VEIL_COLOR = 17
global RING_NUMBER
RING_NUMBER = 18
global RING_TYPE
RING_TYPE = 19
global SPORE_PRINT_COLOR
SPORE_PRINT_COLOR = 20
global POPULATION
POPULATION = 21
global HABITAT
HABITAT = 22

""" Reads a csv file and returns a table (list of lists) """
def read_csv(filename):
    import csv
    with open(filename, 'r') as the_file:
        return [row for row in csv.reader(the_file, dialect='excel') if len(row) > 0]

""" Gets a list of all different categorical values """
def get_categories(table, index):
    return list(set([row[index] for row in table if row[index] != None]))

""" Counts occurences of an element in a column """
def count_occurences(table, index, value):
    return sum([1 for row in table if row[index] == value])

""" Gets all rows where index = value """
def get_instances(table, index, value):
    return [row for row in table if row[index] == value]

""" Partitions the passed in table into k folds """
def partition_into_folds(table, k, class_index):
    folds = [[] for _ in range(k)] # Create disjoint empty lists
    table.sort(key=lambda x: x[class_index]) # Sort by class_index
    for row in table:
        folds[random.choice(range(k))].append(row)

    return folds

""" Creates a testing set and training set from a list of folds, on the index of the test fold """
def create_test_and_train_from_folds(folds, index):
    return [folds[j] for j in range(len(folds)) if j != index], folds[index]

""" Counts occurences of tuples in a table """
def count(table, S):
    return sum([1 if False not in [rule[1] == row[rule[0]] for rule in S] else 0 for row in table])

""" Returns the support for a rule """
def support(table, L, R = None):
    # Generate S = L U R
    S = L
    if R:
        S += R

    # Support(S) = count(S) / N (total)
    return count(table,S) / float(len(table))

""" Returns the confidence for a rule """
def confidence(table, rule):
    # Generate S = L U R 
    S = []
    for tup in rule[0]:
        S.append(tup)
    for tup in rule[1]:
        S.append(tup)
    L = rule[0]
    
    # Calculate count(S) 
    count_S = 0
    for row in table:
        subset = True
        for rule in S:
            if rule[1] != row[rule[0]]:
                subset = False
        if subset == True:
            count_S += 1

    # Calculate count(L)
    count_L = 0
    for row in table:
        subset = True
        for rule in L:
            if rule[1] != row[rule[0]]:
                subset = False
        if subset == True:
            count_L += 1

    # Confidence(S) = count(S) / count(L)
    return count_S / float(count_L)

""" Calculate the lift for a rule """
def lift(rule, table):
    L = (rule[0],)
    R = (rule[1],)
    
    # Lift(A->B) = support(A U B) / ( support(A) * support(B) )
    return support(rule, table) / (support(L, table) * support(R, table))

""" Creates a power set """
# Taken from http://stackoverflow.com/questions/1482308/whats-a-good-way-to-combinate-through-a-set
def powerset(s):
    return [[s[j] for j in range(len(s)) if (i & (1 << j))] for i in range(1 << len(s))]

""" Creates a power set """
def powerset2(s):
    x = len(s)
    ret = []
    for i in range(1 << x):
        up = []
        down = []
        for j in range(x):
            if (i & (1 << j)):
                up.append(s[j])
            else:
                down.append(s[j])
        ret.append((tuple(up),tuple(down)))
    return ret

""" Creates all possible combinations without repeats from groups and in the order they appear"""
def combinations(groups):
    if len(groups) < 2:
        return groups[0]
    return [[e] + c if isinstance(c,list) else [e,c] for c in combinations(groups[1:]) for e in groups[0]]

""" Creates all possible pairs """
def pairs(groups):
    ps = []
    for i,group in enumerate(groups):
        ps += [[e,f] for f in [g for gs in groups[i+1:] for g in gs] for e in group]
    return ps

""" Creates all possible rules given a list of tuples """
def create_rules(raw):
    ret = []
    for i,R in enumerate(raw):
        L = raw[:i] + raw[i+1:]
        ret.append((L,[R]))
        if len(L) > 1:
            ret.append(([R],L))
    return ret

""" Creates all L union R rules from attributes and domains """
def raw_rules(attrs, domains):
    raw = combinations([[(attr,domain) for domain in domains[i]] for i,attr in enumerate(attrs)])
    rules = []
    for base in raw:
        for i,R in enumerate(base):
            L = base[:i] + base[i+1:]
            rules.append((L,[R]))
            rules.append(([R],L))
    return rules

""" Creates a set of rules of the form (L,R) from the passed in table using the passed in attribute indices
    L and R are lists of tuples where each tuple is of the form (index, value) """
def mine_rules(table, attrs):
    return [[not (i in p) for i in attrs] for p in powerset(attrs)[1:-1]]

def mine_rules2(table, attrs):
    if len(attrs) < 2:
        return []
    ret = []
    for i in range(len(attrs)):
        new_attrs = attrs[:i] + attrs[i+1:]
        ret += mine_rules2(table, new_attrs)
        ret += powerset2(new_attrs)[1:-1]
    return set(ret)

def mine_rules3(attrs, domains):
    for i in range(len(attrs)):
        for j in range(len(domains[i])):
            new_domains = []
            # Before current domain
            for k in range(i):
                new_domains += [(attrs[k], d) for d in domains[k]]
                
            # Current domain
            new_domains += [(attrs[i], domains[i][k]) for k in range(len(domains[i])) if k != j]

            # The rest of the domains
            for k in range(i+1,len(domains)):
                new_domains += [(attrs[k], d) for d in domains[k]]
                    
            print (new_domains, [(attrs[i],domains[i][j])])
        print ''

def mine_rules4(attrs, domains):
    if len(attrs) < 2:
        return []
    ret = []
    for i,attr in enumerate(attrs):
        for j,domain in enumerate(domains[i]):
            other_attrs = attrs[:i] + attrs[i+1:]
            other_domains = domains[:i] + domains[i+1:]
            print (attr,domain), other_attrs, other_domains

def mine_rules5(table):
    cats = [get_categories(table,i) for i in range(len(table[0]))]
    return raw_rules(range(len(table[0])),cats)

""" Apriori implementation """
def apriori(table, labels, minsup, minconf):
    """ Creates all supoported itemsets """
    def get_itemsets(table, domains, minsup):
        """ Creates the next possible itemset """
        def next_itemset(items, domains):
            ret = []
            for item in items:
                if item[-1][0] < len(domains) - 1:
                    for a in domains[item[-1][0] + 1]:
                        ret.append(item + [a])
            return ret

        # Create all the itemsets
        itemsets = [[c for c in pairs(domains) if support(table, c) > minsup]]
        next_iter = [item for item in next_itemset(itemsets[-1], domains) if support(table,item) >= minsup]
        while len(next_iter) != 0:
            next_iter = [item for item in next_itemset(itemsets[-1], domains) if support(table,item) >= minsup]
            itemsets.append(next_iter)

        return itemsets
    
    cats = [get_categories(table,i) for i in range(len(table[0]))]
    domains = [[(attr,domain) for domain in cats[i]] for i,attr in enumerate(range(len(table[0])))]

    # Generate all supported itemsets
    return [i for items in get_itemsets(table, domains, minsup) for i in items]
    

""" The main method """
def main():    
    titanic_table = read_csv('titanic.txt')
    titanic_labels = titanic_table[0]
    titanic_table = titanic_table[1:]

    supported = apriori(titanic_table, 0.4)
    mine_rules(table, supported, 0.9)

""" To make this an executable """
if __name__ == '__main__':
    main()
