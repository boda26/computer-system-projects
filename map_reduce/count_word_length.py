#
# Count the length of words in the input files.  Deliminate on all non-word characters.
#
# Example: "data/taylor/*.txt"
#  4: 140
#  3: 123
#  2: 112
#  1: 80

# python3 count_word_length.py data/taylor/*.txt

import sys
from MapReduce import MapReduce

import mappers.map_word_length as map_words
import reducers.reduce_sum as reduce_words

if __name__ == '__main__':
  mr = MapReduce(map_words.map, reduce_words.reduce)
  mr( sys.argv[1:] )