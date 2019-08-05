import tracemalloc
from queue import Queue

import numpy as np

from lobqueue import LobQ
from profiles import generate_lob_data


data = generate_lob_data(appends=1500, levels=5)

print('Running memory profile for queue size of 200')

def total_usage(snapshot, key_type='lineno'):
    snapshot = snapshot.filter_traces((
        tracemalloc.Filter(False, "<frozen importlib._bootstrap>"),
        tracemalloc.Filter(False, "<unknown>"),
    ))
    top_stats = snapshot.statistics(key_type)
    total = sum(stat.size for stat in top_stats)
    return total


# Profiling FastQueue
def lobq():
    q = LobQ(200)
    for item in data:
        q.append(item)

# Profiling python queue
def python_queue():
    q = Queue(1501)
    for item in data:
        q.put(item)

# Profiling numpy queue
def numpy_queue():
    q = np.zeros(200)
    for item in data:
        q = np.append(q, item)


tracemalloc.start()
lobq()
snapshot = tracemalloc.take_snapshot()
tracemalloc.stop()
lobq_mem = total_usage(snapshot)

tracemalloc.start()
python_queue()
snapshot = tracemalloc.take_snapshot()
tracemalloc.stop()
python_mem = total_usage(snapshot)
print(f'Python queue use {python_mem / lobq_mem:.2f}x more memory')

tracemalloc.start()
numpy_queue()
snapshot = tracemalloc.take_snapshot()
tracemalloc.stop()
numpy_mem = total_usage(snapshot)
print(f'Numpy queue use {numpy_mem / lobq_mem:.2f}x more memory')


