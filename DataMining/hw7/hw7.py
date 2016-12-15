"""
Cody Valle, Andrew Burke, Katreina Carpenter
Data Mining Homework 7 Program
"""

# Globals for titanic data
global CLASS; CLASS = 0
global AGE; AGE = 1
global SEX; SEX = 2
global SURVIVED; SURVIVED = 3

# Globals for mushroom data
global CLASS_LABEL; CLASS_LABEL = 0
global CAP_SHAPE; CAP_SHAPE = 1
global CAP_SURFACE; CAP_SURFACE = 2
global CAP_COLOR; CAP_COLOR = 3
global BRUISES; BRUISES = 4
global ODOR; ODOR = 5
global GILL_ATTACHMENT; GILL_ATTACHMENT = 6
global GILL_SPACING; GILL_SPACING = 7
global GILL_SIZE; GILL_SIZE = 8
global GILL_COLOR; GILL_COLOR = 9
global STALK_SHAPE; STALK_SHAPE = 10
global STALK_ROOT; STALK_ROOT = 11
global STALK_SURFACE_ABOVE_RING; STALK_SURFACE_ABOVE_RING = 12
global STALK_SURFACE_BELOW_RING; STALK_SURFACE_BELOW_RING = 13
global STALK_COLOR_ABOVE_RING; STALK_COLOR_ABOVE_RING = 14
global STALK_COLOR_BELOW_RING; STALK_COLOR_BELOW_RING = 15
global VEIL_TYPE; VEIL_TYPE = 16
global VEIL_COLOR; VEIL_COLOR = 17
global RING_NUMBER; RING_NUMBER = 18
global RING_TYPE; RING_TYPE = 19
global SPORE_PRINT_COLOR; SPORE_PRINT_COLOR = 20
global POPULATION; POPULATION = 21
global HABITAT; HABITAT = 22

""" Reads a csv file and returns a table (list of lists) """
def read_csv(filename):
    import csv
    with open(filename, 'r') as the_file:
        return [row for row in csv.reader(the_file, dialect='excel') if len(row) > 0]

""" Gets a list of all different categorical values """
def get_categories(table, index):
    return list(set([row[index] for row in table if row[index] != None]))

""" Counts occurences of tuples in a table """
def count(table, S):
    return sum([1 if False not in [rule[1] == row[rule[0]] for rule in S] else 0 for row in table])

""" Returns the support for a rule """
def support(table, L, R = []):
    return count(table, L + R) / float(len(table))

""" Returns the confidence for a rule """
def confidence(table, L, R):
    return count(table, L + R) / float(count(table, L))

""" Calculate the lift for a rule """
def lift(table, L, R):
    return support(table, L, R) / (support(table, L) * support(table, R))

""" Creates all possible combinations without repeats from groups and in the order they appear"""
def combinations(groups):
    if len(groups) < 2:
        return groups[0]
    return [[e] + c if isinstance(c,list) else [e,c] for c in combinations(groups[1:]) for e in groups[0]]

""" Creates all possible pairs """
def pairs(groups):
    return [[e,f] for i,group in enumerate(groups) for f in [g for gs in groups[i+1:] for g in gs] for e in group]
        
""" Apriori implementation """
def apriori(table, minsup):
    """ Creates all supoported itemsets """
    def get_itemsets(domains):
        """ Creates the next possible itemset """
        def next_itemset(items):
            return [item + [a] for item in items  if (item[-1][0] < len(domains) - 1) for a in domains[item[-1][0] + 1]]

        # Create all the itemsets
        itemsets = [[c for c in pairs(domains) if support(table, c) > minsup]]
        next_iter = [item for item in next_itemset(itemsets[-1]) if support(table,item) >= minsup]
        while len(next_iter) != 0:
            itemsets.append(next_iter)
            next_iter = [item for item in next_itemset(itemsets[-1]) if support(table,item) >= minsup]

        return itemsets
    
    cats = [get_categories(table,i) for i in range(len(table[0]))]
    domains = [[(attr,domain) for domain in cats[i]] for i,attr in enumerate(range(len(table[0])))]

    # Generate all supported itemsets
    return [i for items in get_itemsets(domains) for i in items]

""" Creates a set of rules of the form (L,R) from the passed in table using the passed in attribute indices
    L and R are lists of tuples where each tuple is of the form (index, value) """
def mine_rules(table, labels, supported, minconf):
    """ Creates all possible rules given a list of tuples """
    def create_rules(raw):
        ret = []
        for i,R in enumerate(raw):
            L = raw[:i] + raw[i+1:]
            ret.append((L,[R]))
            if len(L) > 1:
                ret.append(([R],L))
        return ret
    
    """ Beautifies rules """
    def beautify(rule):
        left = ' '.join('%s' % '='.join([labels[x],y]) for x,y in rule[0])
        return left + ' => ' + ' '.join('%s' % '='.join([labels[x],y]) for x,y in rule[1])
    
    # Generate all rules with high enough confidence
    rules = [r for item in supported for r in create_rules(item) if confidence(table,r[0],r[1]) >= minconf]
        
    # Print out the rules         
    for rule in rules:
        print beautify(rule)
        print 'Support:',support(table, rule[0], rule[1])
        print 'Confidence:', confidence(table, rule[0], rule[1])
        print 'Lift:',lift(table, rule[0], rule[1])
        print ''

""" The main method """
def main():    
    titanic_table = read_csv('titanic.txt')

    supported = apriori(titanic_table[1:], 0.4) # Minimum support
    rules = mine_rules(titanic_table[1:], titanic_table[0], supported, 0.9) # Minimum confidence

    mushroom_table = read_csv('agaricus-lepiota.txt')
    mushroom_labels = ['class_label', 'cap_shape', 'cap_surface', 'cap_color', 'bruises', 'odor', 'gill_attachment',
    'gill_spacing', 'gill_size', 'gill_color', 'stalk_shape', 'stalk_root', 'stalk_surface_above_ring',
    'stalk_surface_below_ring', 'stalk_color_above_ring', 'stalk_color_below_ring', 'veil_type', 'veil_color',
    'ring_number', 'ring_type', 'spore_print_color', 'population', 'habitat']

    supported = apriori(mushroom_table, 0.9)
    rules = mine_rules(mushroom_table, mushroom_labels, supported, 0.9)

""" To make this an executable """
if __name__ == '__main__':
    main()
