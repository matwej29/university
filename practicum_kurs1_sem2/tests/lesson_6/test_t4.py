import unittest

import sys
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__ + '/../../'))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from lesson_6.t4_partition import partition


class TestPartition(unittest.TestCase):
    # позитивные тесты
    def test_positive_1(self):
        A = [3, 1, 2, 5, 4]
        partition(A, 0, len(A) - 1)
        self.assertEqual(A, [2, 1, 3, 5, 4])

    def test_positive_2(self):
        A = [3, 2, 1, 4, 5, 0]
        partition(A, 0, 2)
        self.assertEqual(A, [1, 2, 3, 4, 5, 0])

    def test_positive_3(self):
        A = [3, 2, 1, 4, 5, 0]
        partition(A, 3, 5)
        self.assertEqual(A, [3, 2, 1, 0, 4, 5])

    # негативные тесты
    def test_negative_1(self):
        A = 5
        with self.assertRaises(TypeError):
            partition(A, 0, len(A) - 1)

    def test_negative_2(self):
        A = '5'
        with self.assertRaises(TypeError):
            partition(A, 0, len(A) - 1)

    def test_negative_3(self):
        A = [1, 2, 3, 4, 5, 6, 7, 8, 9, '0']
        with self.assertRaises(TypeError):
            partition(A, 0, len(A) - 1)

    # граничные тесты
    def test_boundary_1(self):
        A = []
        partition(A, 0, len(A) - 1)
        self.assertEqual(A, [])

    def test_boundary_2(self):
        A = [1]
        partition(A, 0, len(A) - 1)
        self.assertEqual(A, [1])

    def test_boundary_3(self):
        A = [1, 2]
        partition(A, 0, len(A) - 1)
        self.assertEqual(A, [1, 2])

    def test_boundary_4(self):
        A = [2, 1]
        partition(A, 0, len(A) - 1)
        self.assertEqual(A, [1, 2])

    def test_boundary_5(self):
        A = [1, 1]
        partition(A, 0, len(A) - 1)
        self.assertEqual(A, [1, 1])

    # специальные тесты (со специфическими данными)
    def test_special_1(self):
        A = ['b', 'a', 'f', 'h', '3']
        partition(A, 0, len(A) - 1)
        self.assertEqual(A, ['3', 'a', 'b', 'h', 'f'])


if __name__ == '__main__':
    unittest.main()
