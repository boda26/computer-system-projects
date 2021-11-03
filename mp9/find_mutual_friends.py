#
# Calculate a list of mutual friends for pairs of users.
# (Use a list as the value component of the reduce result.)
#
# Example: "data/graphs/graph-0.txt"
#   (B C): ['D']
#   (B D): ['C']
#   (C D): ['B']
#

# python3 find_mutual_friends.py data/graphs/graph-0.txt

import sys
from MapReduce import MapReduce

import mappers.map_mutual_friends as map_words
import reducers.reduce_sum as reduce_words

if __name__ == '__main__':
  mr = MapReduce(map_words.map, reduce_words.reduce)
  mr( sys.argv[1:] )
