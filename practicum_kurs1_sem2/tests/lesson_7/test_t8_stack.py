import unittest

import sys
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__ + '/../../'))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from lesson_7.t8_stack import MyStack

class TestStack(unittest.TestCase):
    def test_1(self):
        S = MyStack()
        self.assertEqual(S.isEmpty(), True)
        for i in range(10):
            S.push(i)
        for i in range(9, -1, -1):
            self.assertEqual(S.isEmpty(), False)
            self.assertEqual(S.pop(), i)

        self.assertEqual(S.isEmpty(), True)

    def test_2(self):
        S = MyStack()
        self.assertEqual(S.pop(), None)

        S.push(1)
        self.assertEqual(S.pop(), 1)
        S.push(2)
        S.push(3)
        self.assertEqual(S.pop(), 3)
        self.assertEqual(S.pop(), 2)

    def test_3(self):
        S = MyStack()
        for i in range(10):
            self.assertEqual(S.push(i), True)

        self.assertEqual(S.push(10), False)

if __name__ == '__main__':
    unittest.main()
