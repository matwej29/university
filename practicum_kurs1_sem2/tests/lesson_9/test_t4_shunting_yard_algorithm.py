import unittest
import os
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__ + '/../../'))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from lesson_9.t4_shunting_yard_algorithm import shunting_yard_algorithm


class TestShuntingYardAlgorith(unittest.TestCase):
    def test_positive_add_1(self):
        self.assertEqual(' '.join(shunting_yard_algorithm('2 + 2'.split())), '2 2 +')

    def test_positive_subtract_2(self):
        self.assertEqual(' '.join(shunting_yard_algorithm('2 - 3'.split())), '2 3 -')

    def test_positive_multiply_3(self):
        self.assertEqual(' '.join(shunting_yard_algorithm('3 * 4'.split())), '3 4 *')

    def test_positive_divide_4(self):
        self.assertEqual(' '.join(shunting_yard_algorithm('1 / 8'.split())), '1 8 /')

    def test_positive_brackets_5(self):
        self.assertEqual(' '.join(shunting_yard_algorithm('( 2 + 3 ) * 4'.split())), '2 3 + 4 *')

    def test_positive_brackets_6(self):
        self.assertEqual(' '.join(shunting_yard_algorithm('2 + ( 3 * 4 )'.split())), '2 3 4 * +')

    def test_negative_brackets_1(self):
        with self.assertRaises(Exception):
            shunting_yard_algorithm(['('])

        with self.assertRaises(Exception):
            shunting_yard_algorithm([')'])

        with self.assertRaises(Exception):
            shunting_yard_algorithm('( 3 + 4'.split())

        with self.assertRaises(Exception):
            shunting_yard_algorithm('( 4 * * 4'.split())

    # def test_negative_wrong_notation(self):
    #     with self.assertRaises(Exception):
    #         shunting_yard_algorithm('+ 3 4')


if __name__ == '__main__':
    unittest.main()
