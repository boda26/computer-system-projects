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
        for i in range(len(new_lines_split) - 1):
            if new_lines_split[i] != '' and new_lines_split[i] != '':
                bg = new_lines_split[i] + ' ' + new_lines_split[i+1]
                if bg in d:
                    d[bg] += 1
                else:
                    d[bg] = 1
        return d