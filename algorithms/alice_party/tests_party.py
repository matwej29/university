import unittest

from alice_party import alice_party


class AlicePartyTests(unittest.TestCase):
    def test1(self):
        pairs = [(1, 2), (1, 3), (1, 4), (1, 5), (1, 6), (2, 3), (2, 4), (2, 5), (2, 6), (3, 4), (3, 5), (3, 6), (4, 5),
             (4, 6), (5, 6), (7, 8), (7, 9), (7, 10), (7, 11), (7, 12), (8, 9), (8, 10), (8, 11), (8, 12), (9, 10),
             (9, 11), (9, 12), (10, 11), (10, 12), (11, 12)]
        self.assertEqual(alice_party(pairs), 12)

    def test2(self):


if __name__ == '__main__':
    unittest.main()
