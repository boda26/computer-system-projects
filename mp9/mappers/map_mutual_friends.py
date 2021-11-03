#
import itertools

def map(filename):
    with open(filename, "r", encoding="utf-8") as f:
        lines = f.readlines()
        points = []
        pt_dict = {}
        d = {}
        for line in lines:
            points.append(line[0])
            friends_str = line[3:]
            friends_str = friends_str.replace('\n', '')
            friends_list = friends_str.split(',')
            pt_dict[line[0]] = friends_list
        for tp in itertools.combinations(points, 2):
            a = tp[0]
            b = tp[1]
            if a in pt_dict[b]:
                mutual_friend = []
                for af in pt_dict[a]:
                    if af != b and af in pt_dict[b]:
                        mutual_friend.append(af)
                if (len(mutual_friend)) != 0:
                    tp_str = '(' + tp[0] + ' ' + tp[1] + ')'
                    d[tp_str] = mutual_friend
        return d

