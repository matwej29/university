import unittest

from lesson_10.t1_quickfind import QuickFind


class QuickfindTests(unittest.TestCase):
    def test_1(self):
        qf = QuickFind()
        for elem in range(10):
            qf.make_set()
        qf.union_set(4, 3)
        qf.union_set(3, 8)
        qf.union_set(6, 5)
        qf.union_set(9, 4)
        qf.union_set(2, 1)
        self.assertEqual(qf.connected(0, 7), False)
        self.assertEqual(qf.connected(8, 9), True)
        qf.union_set(5, 0)
        qf.union_set(7, 2)
        qf.union_set(6, 1)
        qf.union_set(1, 0)
        self.assertEqual(qf.connected(0, 7), True)
        self.assertEqual(str(qf), '6 6 6 9 9 6 6 6 9 9')


if __name__ == '__main__':
    unittest.main()
