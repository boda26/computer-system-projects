# map function:
def map(filename):
  with open(filename, "r", encoding="utf-8") as f:
    lines = f.read()
    lines = lines.lower()
    d = {}
    for i in range(len(lines)):
      if ord(lines[i]) >= 97 and ord(lines[i]) <= 122:
        if lines[i] not in d:
          d[lines[i]] = 1
        else:
          d[lines[i]] += 1
    return d