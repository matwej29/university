import unittest
import sys
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__ + '/../../'))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from lesson_7.t2_interpolate_search import interpolSearch

class TestInterpolateSearch(unittest.TestCase):
    def test_positive_1(self):
        self.assertEqual(interpolSearch([1, 2, 3], 2), [2])

    def test_positive_2(self):
        self.assertEqual(interpolSearch([1, 2, 7, 8], 7), [8, 7])


if __name__ == '__main__':
    unittest.main()
