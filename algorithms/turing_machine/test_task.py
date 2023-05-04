import unittest
from turing_machine import Turing


def start(raw_input):
    with open("res") as f:
        lines = f.readlines()

    mT = Turing()
    mT.set_program(lines)
    return mT.start(''.join(list(map(lambda x: 'E' if x == " " else x, raw_input))))


class TaskTests(unittest.TestCase):
    def test1(self):
        self.assertEqual(start('1 0 1001 11 101 1 0'), '1 1001 11 101 1 0')

    def test2(self):
        self.assertEqual(start('1 0 101 11 1001 1 0'), '1 101 11 1001 1 0')

    def test3(self):
        self.assertEqual(start('1001'), '1001')

    def test4(self):
        self.assertEqual(start('1 0 0 1 0 0 111 10001'), '1 0 0 1 0 0 111 10001')


if __name__ == '__main__':
    import reformat_file

    unittest.main()
