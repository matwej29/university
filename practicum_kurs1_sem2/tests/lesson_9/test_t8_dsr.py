import unittest

from lesson_9.t4_shunting_yard_algorithm import shunting_yard_algorithm
from lesson_9.t8_dsr import abstract_syntax_tree, str_abstract_tree


class TestAbstractSyntaxTree(unittest.TestCase):
    def test_positive_1(self):
        inf = '2 + 3 * 4 - 5'.split()
        ops = shunting_yard_algorithm(inf)
        res = str_abstract_tree(abstract_syntax_tree(ops))
        self.assertEqual(res, '(- (+ (2 () ()) (* (3 () ()) (4 () ()))) (5 () ()))')

    def test_positive_2(self):
        inf = '1 - 2'.split()
        ops = shunting_yard_algorithm(inf)
        res = str_abstract_tree(abstract_syntax_tree(ops))
        self.assertEqual(res, '(- (1 () ()) (2 () ()))')

    def test_positive_3(self):
        inf = '( 1 + 2 ) * 3'.split()
        ops = shunting_yard_algorithm(inf)
        res = str_abstract_tree(abstract_syntax_tree(ops))
        self.assertEqual(res, '(* (+ (1 () ()) (2 () ())) (3 () ()))')

    def test_positive_4(self):
        inf = '( 2 ** 3 ) ** 4'.split()
        ops = shunting_yard_algorithm(inf)
        res = str_abstract_tree(abstract_syntax_tree(ops))
        self.assertEqual(res, '(** (** (2 () ()) (3 () ()) (4 () ()))')


    def test_special_1(self):
        inf = ''.split()
        ops = shunting_yard_algorithm(inf)
        res = str_abstract_tree(abstract_syntax_tree(ops))
        self.assertEqual(res, '()')

if __name__ == '__main__':
    unittest.main()
