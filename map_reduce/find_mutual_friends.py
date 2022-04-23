#
# Calculate a list of mutual friends for pairs of users.
# (Use a list as the value component of the reduce result.)
#
# Example: "data/graph0/*.txt"
#   (B C): ['D']
#   (B D): ['C']
#   (C D): ['B']
#

# python3 find_mutual_friends.py data/graph0/*.txt

import sys
from MapReduce import MapReduce

import mappers.map_mutual_friends as map_mutual_friends
import reducers.reduce_mutual_friends as reduce_mutual_friends

if __name__ == '__main__':
  mr = MapReduce(map_mutual_friends.map, reduce_mutual_friends.reduce)
  mr( sys.argv[1:] )
