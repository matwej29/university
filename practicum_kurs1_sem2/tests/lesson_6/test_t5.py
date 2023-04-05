import unittest

import sys
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__ + '/../../'))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from practicum_kurs1_sem2.lesson_6.t5_quick_sort import quickSort

class TestQuickSort(unittest.TestCase):
    def test_quick_sort(self):
        A = [1, 2, 3, 4, 5]
        quickSort(A)
        self.assertEqual(A, [1, 2, 3, 4, 5])

        A = [5, 4, 3, 2, 1]
        quickSort(A)
        self.assertEqual(A, [1, 2, 3, 4, 5])

        A = [1, 1, 1, 1, 1]
        quickSort(A)
        self.assertEqual(A, [1, 1, 1, 1, 1])

        A = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        quickSort(A)
        self.assertEqual(A, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10])

        A = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
        quickSort(A)
        self.assertEqual(A, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10])

    def test_positive_1(self):
        A = [1, 2, 3, 4, 5, 6, 7, 8, 9]
        quickSort(A)
        self.assertEqual(A, [1, 2, 3, 4, 5, 6, 7, 8, 9])

    def test_positive_2(self):
        A = [9, 8, 7, 6, 5, 4, 3, 2, 1]
        quickSort(A)
        self.assertEqual(A, [1, 2, 3, 4, 5, 6, 7, 8, 9])

    def test_positive_3(self):
        A = [1, 1, 1, 1, 1, 1, 1, 1, 1]
        quickSort(A)
        self.assertEqual(A, [1, 1, 1, 1, 1, 1, 1, 1, 1])

    def test_positive_4(self):
        A = [1, 2, 3, 4, 5, 6, 7, 8, 9, 0]
        quickSort(A)
        self.assertEqual(A, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])

    # негативные (с некорректными входными данными)
    def test_negative_1(self):
        A = 5
        with self.assertRaises(TypeError):
            quickSort(A)

    def test_negative_2(self):
        A = '5'
        with self.assertRaises(TypeError):
            quickSort(A)

    def test_negative_3(self):
        A = [1, 2, 3, 4, 5, 6, 7, 8, 9, '0']
        with self.assertRaises(TypeError):
            quickSort(A)

    # граничные (с крайними значениями)
    def test_boundary_1(self):
        A = []
        quickSort(A)
        self.assertEqual(A, [])

    def test_boundary_2(self):
        A = [1]
        quickSort(A)
        self.assertEqual(A, [1])

    def test_boundary_3(self):
        A = [1, 2]
        quickSort(A)
        self.assertEqual(A, [1, 2])

    def test_boundary_4(self):
        A = [2, 1]
        quickSort(A)
        self.assertEqual(A, [1, 2])

    # специальные (со специфическими значениями)
    def test_special_1(self):
        A = [1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0]
        quickSort(A)
        self.assertEqual(A, [0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9])

    def test_special_2(self):
        A = [1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0]
        quickSort(A)
        self.assertEqual(A, [0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1])

    def test_special_3(self):
        A = [-1, -2, -3, -4, -5, -6, -7, -8, -9, 0, 0, 0, 0, 0]
        quickSort(A)
        self.assertEqual(A, [-9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 0, 0, 0, 0])


if __name__ == '__main__':
    unittest.main()
