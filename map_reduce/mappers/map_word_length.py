#
def map(filename):
    with open(filename, "r", encoding="utf-8") as f:
        d = {}
        new_lines = ''
        lines = f.readlines()
        for line in lines:
            line = line.lower()
            for i in range(len(line)):
                if (ord(line[i]) >= 97 and ord(line[i]) <= 122) or ord(line[i]) == 32:
                    new_lines += line[i]
                else:
                    new_lines += ' '
            new_lines += ' '
        new_lines_split = new_lines.split(' ')
        for word in new_lines_split:
            if word != '':
                if str(len(word)) not in d:
                    sl = str(len(word))
                    d[sl] = 1
                else:
                    d[str(len(word))] += 1
        return d