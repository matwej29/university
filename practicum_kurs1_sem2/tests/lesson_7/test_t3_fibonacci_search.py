import unittest
import os
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__ + '/../../'))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from lesson_7.t3_fibonacci_search import fibSearch


class TestFibonacciSearch(unittest.TestCase):
    def test_positive_1(self):
        self.assertEqual(fibSearch([1, 2, 3], 3), [1, 2, 3])

    def test_positive_2(self):
        self.assertEqual(fibSearch([1, 2, 3], 1), [1])

    def test_negative_1(self):
        self.assertEqual(fibSearch([3, 4, 7, 10, 13, 16, 18, 19], 8), [7, 13, 10, 10])

    def test_negative_2(self):
        self.assertEqual(fibSearch([0, 1], -1), [None])


if __name__ == '__main__':
    unittest.main()
