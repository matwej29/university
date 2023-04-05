import unittest

import sys
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__ + '/../../'))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from lesson_7.t9_queue import MyQueue

class TestQueue(unittest.TestCase):
    def test_1(self):
        Q = MyQueue()
        self.assertEqual(Q.isEmpty(), True)
        for i in range(10):
            Q.enqueue(i)
        for i in range(10):
            self.assertEqual(Q.isEmpty(), False)
            self.assertEqual(Q.dequeue(), i)

        self.assertEqual(Q.isEmpty(), True)

    def test_2(self):
        Q = MyQueue()
        self.assertEqual(Q.dequeue(), None)

        Q.enqueue(1)
        self.assertEqual(Q.dequeue(), 1)
        Q.enqueue(2)
        Q.enqueue(3)
        self.assertEqual(Q.dequeue(), 2)
        self.assertEqual(Q.dequeue(), 3)

    def test_3(self):
        Q = MyQueue()
        for i in range(10):
            self.assertEqual(Q.enqueue(i), True)

        self.assertEqual(Q.enqueue(10), False)

if __name__ == '__main__':
    unittest.main()
