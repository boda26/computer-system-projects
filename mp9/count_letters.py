#
# Count letters in the input files.  Ignore all non-letters and count as lower-case.
#
# Example: "data/books/*.txt"
#   e: 207109
#   t: 156014
#   a: 135884
#   o: 125416
#   i: 116450
#   n: 113747
#   h: 110008
#   s: 108394
#

import sys
from MapReduce import MapReduce

import mappers.map_letters as map_letters
import reducers.reduce_sum as reduce_sum

if __name__ == '__main__':
  mr = MapReduce(map_letters.map, reduce_sum.reduce)
  mr( sys.argv[1:] )
