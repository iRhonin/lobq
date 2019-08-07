from sys import getrefcount

import pytest

from lobq import LobQ as Queue


def test_lobq():
    q = Queue(2)
    lobs = []

    for i in range(3 * q.cap):
        lob = dict(time=f'{i}', bid_vol=[1], bid_pr=[1], ask_vol=[1], ask_pr=[1])

        refs = getrefcount(lob)
        q.append(lob)
        assert getrefcount(lob) == refs + 1

        lobs.append(lob)

    assert q[0] == lobs[-1]
    assert q[-1] == lobs[2 * q.cap]

    last_item = q[-1]
    item_time = last_item['time']
    assert q.peek(item_time) == last_item

    peeked = q.peek('None')
    assert peeked is None

    del lobs
    del q


def test_lobq_multithread():
    import threading

    q = Queue(2)

    def append_and_peek():
        for i in range(3 * q.cap):
            lob = dict(time=f'{i}', bid_vol=[1], bid_pr=[1], ask_vol=[1], ask_pr=[1])

            refs = getrefcount(lob)
            q.append(lob)
            assert getrefcount(lob) == refs + 1
            assert q[0] == lob
            assert q.peek(lob['time']) == lob

    threads = []
    for i in range(100):
        t = threading.Thread(target=append_and_peek)
        threads.append(t)

    for t in threads:
        t.start()

    for t in threads:
        t.join()


def test_lobq_api():
    q = Queue(1)

    with pytest.raises(TypeError):
        assert q.append(some_key=0)

    with pytest.raises(TypeError):
        assert q.append(1, 2)


test_lobq()
test_lobq_multithread()
test_lobq_api()
