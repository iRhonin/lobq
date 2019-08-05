from datetime import datetime
from random import random


def genearate_double_random_array(size):
    return [random() for i in range(size)]

def generate_lob_data(appends, levels):
    return [
        dict(
            time=str(datetime.utcnow()),
            ask_pr=genearate_double_random_array(levels),
            ask_vol=genearate_double_random_array(levels),
            bid_pr=genearate_double_random_array(levels),
            bid_vol=genearate_double_random_array(levels),
        )
        for i in range(appends)
    ]


