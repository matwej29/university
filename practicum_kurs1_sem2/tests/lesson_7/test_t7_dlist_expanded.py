import unittest

import sys
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__ + '/../../'))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from lesson_7.t7_dlist_expanded import MyList


class TestExpandedDoublyLinkedList(unittest.TestCase):
    def test_get_elem(self):
        l = MyList()
        l.append(1)
        l.append(2)
        elem = l.find(1)
        l.addAfter(elem, 3)
        self.assertEqual(str(l), '[1, 3, 2]')

    def test_set_elem(self):
        l = MyList()
        l.append(1)
        l.append(2)
        elem = l.find(1)
        l.addAfter(elem, 3)
        l[1] = 4
        self.assertEqual(str(l), '[1, 4, 2]')

    def test_find_elem(self):
        l = MyList()
        l.append(1)
        l.append(2)
        self.assertEqual(l.find(2).data, 2)
        elem = l.find(1)
        l.addAfter(elem, 3)
        self.assertEqual(l.find(3).data, 3)
        l[1] = 4
        self.assertEqual(l.find(4).data, 4)

    def test_contains(self):
        l = MyList()
        l.append(1)
        l.append(2)
        elem = l.find(1)
        l.addAfter(elem, 3)
        l[1] = 4

        self.assertEqual(4 in l, True)
        self.assertEqual(0 in l, False)

    def test_add(self):
        l1 = MyList()
        l2 = MyList()
        for i in range(10):
            l1.append(i)
        for i in range(100):
            l2.append(i)

        l3 = MyList()
        for i in range(10):
            l3.append(i)
        for i in range(100):
            l3.append(i)

        added = l1 + l2
        for i in range(110):
            self.assertEqual(added[i], l3[i])

    def test_concat(self):
        l1 = MyList()
        l2 = MyList()
        for i in range(10):
            l1.append(i)
        for i in range(100):
            l2.append(i)

        l3 = MyList()
        for i in range(10):
            l3.append(i)
        for i in range(100):
            l3.append(i)

        l1.concat(l2)
        for i in range(110):
            self.assertEqual(l1[i], l3[i])

    def test_iter(self):
        l1 = MyList()
        for i in range(10):
            l1.append(i)

        self.assertEqual([x for x in l1], [x for x in range(10)])


if __name__ == '__main__':
    unittest.main()
