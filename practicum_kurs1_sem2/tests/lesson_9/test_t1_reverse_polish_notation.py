import unittest

import sys
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__ + '/../../'))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from lesson_9.t1_reverse_polish_notation import calculate_ops


class TestRPN(unittest.TestCase):
    def test_positive_1(self):
        self.assertEqual(calculate_ops('2 2 +'.split()), 4)

    def test_positive_2(self):
        self.assertEqual(calculate_ops('3 1 -'.split()), 2)

    def test_positive_3(self):
        self.assertEqual(calculate_ops('2 2 + 3 1 - *'.split()), 8)

    def test_positive_4(self):
        self.assertEqual(calculate_ops('2 1 - 4 5 - + 8 **'.split()), 0)

    def test_negative_1(self):
        with self.assertRaises(ZeroDivisionError):
            calculate_ops('1 0 /'.split())

    def test_negative_2(self):
        self.assertEqual(calculate_ops('1 *'.split()), 'error')

    def test_negative_3(self):
        self.assertEqual(calculate_ops('1 1'.split()), 'error')

    def test_negative_4(self):
        self.assertEqual(calculate_ops(''.split()), 'error')


if __name__ == '__main__':
    unittest.main()
