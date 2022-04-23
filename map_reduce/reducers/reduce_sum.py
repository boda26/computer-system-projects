# reduce function:
def reduce(left, right):
  for key, value in left.items():
    if key in right:
      right[key] += value
    else:
      right[key] = value
  return right
