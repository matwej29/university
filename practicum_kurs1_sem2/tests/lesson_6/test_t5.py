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


if __name__ == '__main__':
    unittest.main()