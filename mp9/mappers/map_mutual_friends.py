#
import itertools

def map(filename):
    with open(filename, "r", encoding="utf-8") as f:
        d = {}
        lines = f.read()
        main_char = lines[0]
        friends = lines[3:]
        friends = friends.replace('\n', '')
        friend_list  = friends.split(',')
        for f in friend_list:
            if (main_char < f):
                tps = '(' + main_char + ' ' + f + ')'
            else:
                tps = '(' + f + ' ' + main_char + ')'
            value_list = []
            for f2 in friend_list:
                if f2 != f:
                    value_list.append(f2)
            d[tps] = value_list
        return d
