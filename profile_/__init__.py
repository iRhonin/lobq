import timeit
import tracemalloc
from datetime import datetime
from functools import partial
from random import random

import pandas as pd


def genearate_double_random_array(size):
    return [random() for i in range(size)]


def generate_lob_data(size, levels):
    return [
        dict(
            time=str(datetime.utcnow()),
            ask_pr=genearate_double_random_array(levels),
            ask_vol=genearate_double_random_array(levels),
            bid_pr=genearate_double_random_array(levels),
            bid_vol=genearate_double_random_array(levels),
        )
        for i in range(size)
    ]


def profile_func(fn, type_, q_size, data_size, levels, runs):

    time = timeit.Timer(fn).timeit(runs) / runs

    result = pd.DataFrame(
        dict(
            type_=[type_],
            time=[time],
            time_per_loop=[time / data_size],
            q_size=q_size,
            data_size=[data_size],
            levels=[levels],
            runs=[runs],
        )
    )

    return result


def profile(q_size, data_size, levels, runs=10):

    # C_API
    from lobq import LobQ

    def lobq():
        q = LobQ(q_size)
        for item in data:
            q.append(item)

    # Pure Python
    from pure_python import Queue as PythonQueue

    def pure_python():
        q = PythonQueue(q_size)
        for item in data:
            q.append(item)

    # Compiled Python
    from cyq import Queue as CyQueue  # Compiled by cython

    def cython():
        q = CyQueue(q_size)
        for item in data:
            q.append(item)

    # Builtin queue
    from queue import Queue

    def builtin_queue():
        q = Queue(q_size)
        for item in data:
            if q.full():
                q.get()
            q.put(item)

    # Numpy queue
    from numpy_queue import Queue as NumpyQueue

    def numpy_queue():
        q = NumpyQueue(q_size)
        for item in data:
            q.append(item)

    types = (
        ('C API', lobq),
        ('Pure Python', pure_python),
        ('Cython', cython),
        ('Builtin Queue', builtin_queue),
        ('Numpy', numpy_queue),
    )

    generate_data = partial(generate_lob_data, size=data_size, levels=levels)

    results = pd.DataFrame()

    for type_, fn in types:
        data = generate_data()
        res = profile_func(fn, type_, q_size, data_size, levels, runs)
        results = results.append(res, ignore_index=True)

    return results
