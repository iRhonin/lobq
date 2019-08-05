from fastqueue import Queue


def test_peek():
    q = Queue(10)

    lob1 = dict(
        time='1',
        bid_vol=[1],
        bid_pr=[1],
        ask_vol=[1],
        ask_pr=[1],
    )
    lob2 = dict(
        time='2',
        bid_vol=[2],
        bid_pr=[2],
        ask_vol=[2],
        ask_pr=[2],
    )

    print(q.append(lob1))
    print(q.append(lob2))

    peeked = q.peek('1')
    assert peeked == lob1

    peeked = q.peek('2')
    assert peeked == lob2

    peeked = q.peek('none')
    assert peeked is None
