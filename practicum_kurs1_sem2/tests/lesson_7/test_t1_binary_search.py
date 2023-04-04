import unittest

import os
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__ + '/../../'))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from lesson_7.t1_binary_search import binSearch


class TestBinarySearch(unittest.TestCase):
    def test_positive_1(self):
        self.assertEqual(binSearch([1, 2, 3, 4, 5], 3), [3])

    def test_positive_2(self):
        self.assertEqual(binSearch([1, 2, 3, 4, 5], 1), [3, 1])

    def test_positive_3(self):
        self.assertEqual(binSearch([1, 2, 3, 4, 5], 5), [3, 4, 5])

    def test_negative_1(self):
        self.assertEqual(binSearch([1, 2, 3, 4, 5], 6), [None])

    def test_negative_2(self):
        self.assertEqual(binSearch([1, 2, 3, 4, 5], 0), [None])



if __name__ == "__main__":
    unittest.main()
