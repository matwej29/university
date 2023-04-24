import unittest

from lesson_9.t5_prefix_suffix import preorder_traversal, postorder_traversal


class TestTraversals(unittest.TestCase):
    def test_traversal(self):
        r = '5'
        edges = ['1 2',
                 '2 3',
                 '2 5',
                 '5 9',
                 '9 7',
                 '7 6',
                 '7 8',
                 '9 10']
        graph = dict()
        for edge in edges:
            edge = edge.split()
            if edge[0] not in graph:
                graph[edge[0]] = list()
            if edge[1] not in graph:
                graph[edge[1]] = list()

            graph[edge[0]].append(edge[1])
            graph[edge[1]].append(edge[0])

        self.assertEqual(' '.join(preorder_traversal(graph, r)), '5 2 1 3 9 7 6 8 10')
        self.assertEqual(' '.join(postorder_traversal(graph, r)), '1 3 2 6 8 7 10 9 5')


if __name__ == '__main__':
    unittest.main()
