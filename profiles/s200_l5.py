import timeit
from queue import Queue

import numpy as np

from lobqueue import LobQ
from numpy_queue import Queue as NumpyQueue
from pure_python import Queue as PythonQueue
from cyq import Queue as CyQueue # Compiled by cython
from profiles import generate_lob_data


Q_SIZE = 200
APPENDS = 1500
LEVELS = 5

data = generate_lob_data(appends=APPENDS, levels=LEVELS)

print('CPU profile: '
    f'\n\tQueue Size: {Q_SIZE}'
    f'\n\tData Size: {APPENDS}'
    f'\n\tLevels: {LEVELS}'
    '\n'
)

# Profiling FastQueue
def lobq():
    q = LobQ(Q_SIZE)
    for item in data:
        q.append(item)
t_lobq = timeit.Timer(lobq).timeit(10)
print(f'Cpython: {t_lobq * (1000 * 1000):.3f}us = 1x')

# Profiling Pure Python
def pure_python():
    q = PythonQueue(Q_SIZE)
    for item in data:
        q.append(item)
t_python = timeit.Timer(pure_python).timeit(10)
print(f'PurePython: {t_python / t_lobq:.2f}x slower')

# Profiling Compiled Python
def cython():
    q = CyQueue(Q_SIZE)
    for item in data:
        q.append(item)
t_cython = timeit.Timer(cython).timeit(10)
print(f'CompiledPython: {t_cython / t_lobq:.2f}x slower')

# Profiling python queue
def python_queue():
    q = Queue(Q_SIZE) # Because
    for item in data:
        if(q.full()):
            q.get()
        q.put(item)

t = timeit.Timer(python_queue).timeit(10)
print(f'Bultin: {t / t_lobq:.2f}x slower')

def numpy_queue():
    q = NumpyQueue(Q_SIZE)
    for item in data:
       q.append(item)

t = timeit.Timer(numpy_queue).timeit(10)
print(f'Numpy: {t / t_lobq:.2f}x slower')

