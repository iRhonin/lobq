import timeit
from queue import Queue

import numpy as np

from lobqueue import LobQ
from profiles import generate_lob_data


data = generate_lob_data(appends=1500, levels=5)


# Profiling FastQueue
def lobq():
    q = LobQ(200)
    for item in data:
        q.append(item)

t_lobq = timeit.Timer(lobq).timeit(10)

# Profiling python queue
def python_queue():
    q = Queue(1500)
    for item in data:
        q.put(item)

t = timeit.Timer(python_queue).timeit(10)
print(f'Python queue is {t / t_lobq:.2f}x slower')

def numpy_queue():
    q = np.zeros(200)
    for item in data:
       q = np.append(q, item)

t = timeit.Timer(numpy_queue).timeit(10)
print(f'numpy queue is {t / t_lobq:.2f}x slower')


