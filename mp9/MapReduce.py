# MapReduce class design from: https://pymotw.com/2/multiprocessing/mapreduce.html
# - Modified for native Python dict usage
# - Modified for glob file input
# - Modified for flattening maps
# - Modified for printing output

import multiprocessing
import glob

NUM_WORKERS = 10

class MapReduce(object):
    def __init__(self, map_func, reduce_func):
        # Function for the map phase
        self.map_func = map_func

        # Function for the reduce phase
        self.reduce_func = reduce_func

        # Pool of processes to parallelize computation
        self.proccess_pool = multiprocessing.Pool(NUM_WORKERS)

    def kv_sort(self, mapped_values : list[dict]) -> dict:
        # https://stackoverflow.com/questions/613183/how-do-i-sort-a-dictionary-by-value
        return {k: v for k, v in sorted(mapped_values.items(), key=lambda item: item[1], reverse=True)}

    def reduce(self, map_phase : list[dict]) -> dict:
        d = map_phase

        while len(d) > 1:
            # Convert the list of dicts to tuples for `self.proccess_pool.starmap`:
            it = iter(d)
            tuple_list = list(zip(it, it))

            # Ensure we don't lose the last one when we have an odd number of elements:
            if len(d) % 2 == 1: last_element = d[-1]
            else              : last_element = None

            # Run this reduce phase in our process pool:
            d = self.proccess_pool.starmap(self.reduce_func, tuple_list, 2)
            if last_element: d.append(last_element)
            
        return d[0]

    def __call__(self, data_in : list[str]) -> dict:
        # Error checking:
        if len(data_in) == 0:
            raise Exception("No data provided.")

        files = []
        for file in data_in:
            files += glob.glob(file)

        # Run the map phase in our process pool:
        map_phase = self.proccess_pool.map(self.map_func, files)

        flat_map = []
        for item in map_phase:
            if isinstance(item, list):
                for e in item:
                    flat_map.append(e)
            else:
                flat_map.append(item)

        result = self.reduce(flat_map)

        # Sort the resulting mapped data:
        result = self.kv_sort(result)

        # Print out up to the first 20 key/values:
        i = 0
        for key in result:
            value = result[key]
            print(f"{key}: {value}")
            i += 1
            if i == 20: break

        # Return the results
        return result