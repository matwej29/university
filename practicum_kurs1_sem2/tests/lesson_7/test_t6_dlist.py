import unittest

import sys
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__ + '/../../'))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from practicum_kurs1_sem2.lesson_7.t6_dlist import MyList


class MyListTest(unittest.TestCase):
    def test1(self):
        lst = MyList()
        lst.append(5)
        lst.append(5)
        lst.append(5)
        self.assertEqual(len(lst), 3)
        self.assertEqual(str(lst), '[5, 5, 5]')
        lst.pop()
        lst.pop()
        lst.pop()
        self.assertEqual(len(lst), 0)

    def test2(self):
        lst = MyList()
        lst.append(1)
        lst.append(2)
        self.assertEqual(lst.popFirst(), 1)
        self.assertEqual(lst.pop(), 2)

    def test3(self):
        lst = MyList()
        lst.pushFirst(3)
        lst.pushFirst(2)
        lst.pushFirst(1)
        self.assertEqual(str(lst), '[1, 2, 3]')
        self.assertEqual(lst.pop(), 3)
        self.assertEqual(lst.popFirst(), 1)
        self.assertEqual(len(lst), 1)
        lst.append(5)
        self.assertEqual(lst.popFirst(), 2)
        lst.popFirst()
        self.assertEqual(len(lst), 0)
        with self.assertRaises(Exception):
            lst.pop()


if __name__ == '__main__':
    unittest.main()
