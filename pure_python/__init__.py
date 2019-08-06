

class Queue:
    def __init__(self, cap: int):
        self.cap = cap
        self.queue = [None] * cap
        self.head = -1
        self.tail = 0

    def __len__(self):
        if(self.head == -1):
            return 0;
        elif(self.head >= self.tail):
            return self.head - self.tail + 1;
        else:
            return self.capacity;

    def __getitem__(self, i):
        return self.queue[(self.head - i) % self.cap]

    def append(self, item):
        self.head += 1
        self.head %= self.cap

        if self.head == self.tail and len(self) == self.cap:
            self.tail += 1
            self.tail %= self.cap

        self.queue[self.head] = item

