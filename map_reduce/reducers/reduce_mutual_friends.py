#
def reduce(left, right):
    for key1, value1 in left.items():
        if key1 in right:
            if value1 == []:
                right.pop(key1)
                continue
            value2 = right[key1]
            for v in value2:
                if v not in value1:
                    value2.remove(v)
        else:
            right[key1] = value1
    for k2, v2 in right.copy().items():
        if v2 == []:
            right.pop(k2)
    return right
