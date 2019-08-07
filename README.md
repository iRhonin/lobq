# Project description

## Fast Lob Queue
Implemented in C and complied in order to achive high performance, 5x time faster
than [numpy implementation](lobq_profiler/numpy_queue/__init__.py).

See [profile documention](lobq_profiler/README.md) for more benchmarks.

### Installation (Python >= 3.6)

`$ git clone git@github.com:ArashFatahzade/lobq.git`

or

`$ git clone https://github.com/ArashFatahzade/lobq.git`

```
$ cd lobq/
$ pip3 install cython  # Can not intall via dependencies
$ pip3 install -e .  # Add a sudo before the command for global
```

### Tests

`$ pytest -v # -v for verbos log`


### Usage

```python3
from lobq import LobQ

q = LobQ(cap=10)  # Queue Size 

lob_item = dict(
    time='2016-01-11T00:12:40',
    ask_pr=[1.17, 1.20, 1.25],
    bid_pr=[1.21, 1.20, 1.19], 
    ask_vol=[5.4, 4.2, 1.1],
    bid_vol=[2.4, 1.2, 4.1],
)

q.append(lob_item)

len(q)

q.peek(lob_item['time'])

q[0]  # Newest item
q[-1]  # Oldest item
```
