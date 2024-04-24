import numpy as np
import unittest

from ff import Graph


# https://en.wikipedia.org/wiki/Edmonds%E2%80%93Karp_algorithm
wiki_graph = [
    [0, 3, 0, 3, 0, 0, 0],
    [0, 0, 4, 0, 0, 0, 0],
    [3, 0, 0, 1, 2, 0, 0],
    [0, 0, 0, 0, 2, 6, 0],
    [0, 1, 0, 0, 0, 0, 1],
    [0, 0, 0, 0, 0, 0, 9],
    [0, 0, 0, 0, 0, 0, 0]
]
max_wiki_flow = [
    [0, 2, 0, 3, 0, 0, 0],
    [0, 0, 2, 0, 0, 0, 0],
    [0, 0, 0, 1, 1, 0, 0],
    [0, 0, 0, 0, 0, 4, 0],
    [0, 0, 0, 0, 0, 0, 1],
    [0, 0, 0, 0, 0, 0, 4],
    [0, 0, 0, 0, 0, 0, 0]
]

graph_1 = [
    [0, 8, 0, 0, 3, 0],
    [0, 0, 9, 0, 0, 0],
    [0, 0, 0, 0, 7, 4],
    [0, 0, 0, 0, 0, 5],
    [0, 0, 0, 4, 0, 0],
    [0, 0, 0, 0, 0, 0]
]

graph_2 = [
    [0, 67, 0, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 99, 64, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 84, 0, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 19, 16, 89, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 86, 77, 0, 77, 0, 0, 68],
    [93, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 9, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 17, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 0, 0, 0, 9],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34, 32, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 98, 0, 83, 0, 0, 6, 94, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 0, 0, 0, 0, 82, 84],
    [0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 95, 0, 0, 0, 0]
]

graph_3 = [
    [0, 16, 13, 0, 0, 0],
    [0, 10, 0, 12, 0, 0],
    [0, 4, 0, 0, 14, 0],
    [0, 0, 9, 0, 0, 20],
    [0, 0, 0, 7, 0, 4],
    [0, 0, 0, 0, 0, 0]
]

graph_4 = [
    [0, 16, 13, 0, 0, 0],
    [0, 0, 10, 12, 0, 0],
    [0, 0, 0, 0, 14, 0],
    [0, 0, 9, 0, 0, 20],
    [0, 0, 0, 7, 0, 4],
    [0, 0, 0, 0, 0, 0]
]


class TestAssignment06Student(unittest.TestCase):

    def check_ff(self, graph, source, sink, expected_flow):
        flow = Graph(graph).ford_fulkerson(source, sink)
        self.assertEqual(expected_flow, flow, f"max flow should be {expected_flow} but was: {flow}")

    def check_ff_step(self, graph, source, sink, expected_flow):
        g = Graph(graph)
        flow = 0
        residual_graph = [[j for j in i] for i in graph]

        path_flow = g.ff_step(source, sink)
        while path_flow > 0:
            flow += path_flow
            arr = np.array(g.latest_augmenting_path)
            values = np.unique(np.absolute(arr))[1:]  # all non-zero flow values
            self.assertEqual(1, values.size, f"current augmenting path should have a constant value but has multiple: "
                             f"{values}")
            sum_rows = arr.sum(axis=1)
            sum_cols = arr.sum(axis=0)
            for i in range(len(graph)):
                if i == source:
                    self.assertEqual(0, sum_cols[i], "incoming flow of source is not 0")
                    self.assertEqual(path_flow, sum_rows[i], f"outgoing flow of source is not {path_flow}")
                elif i == sink:
                    self.assertEqual(path_flow, sum_cols[i], f"incoming flow of sink is not {path_flow}")
                    self.assertEqual(0, sum_rows[i], "outgoing flow of sink is not 0")
                else:
                    self.assertEqual(sum_cols[i], sum_rows[i], f"flow is not maintained in node {i}, "
                                                               f"in: {sum_cols[i]}, out: {sum_rows[i]}")
            self.assertTrue(g.latest_augmenting_path <= residual_graph,
                            f"current augmenting path should comply with residuals \n{g.latest_augmenting_path}\n\n\t")
            self.assertTrue(g.current_flow <= g.graph,
                            f"current flow should comply with capacities \n{g.current_flow}\n\n\t")
            residual_graph = [[j for j in i] for i in g.residual_graph]
            path_flow = g.ff_step(source, sink)

        self.assertEqual(expected_flow, flow, f"max flow should be {expected_flow} but was: {flow}")

    def test_ff_wiki(self):
        g = Graph(wiki_graph)
        source = 0
        sink = 6
        flow = g.ford_fulkerson(source, sink)

        self.assertEqual(5, flow, f"max flow should be 5 but was: {flow}")
        self.assertEqual(True, g.current_flow <= g.graph,
                         f"max flow should comply with capacities \n{g.current_flow}\n\n\t")

        for node in range(len(max_wiki_flow)):
            self.assertEqual(g.current_flow[node], max_wiki_flow[node],
                             f"flow of node {node} incorrect ({g.current_flow[node]} != {max_wiki_flow[node]})")

    def test_ff_1(self):
        self.check_ff(graph_1, 0, 5, 8)

    def test_ff_step_1(self):
        self.check_ff_step(graph_1, 0, 5, 8)
        self.check_ff_step(wiki_graph, 0, 6, 5)

    def test_ff_2(self):
        self.check_ff(graph_2, 0, 15, 41)

    def test_ff_step_2(self):
        self.check_ff_step(graph_2, 0, 15, 41)

    def test_ff_3(self):
        self.check_ff(graph_3, 0, 5, 23)

    def test_ff_step_3(self):
        self.check_ff_step(graph_3, 0, 5, 23)

    def test_ff_4(self):
        self.check_ff(graph_4, 0, 5, 23)

    def test_ff_step_4(self):
        self.check_ff_step(graph_4, 0, 5, 23)


if __name__ == '__main__':
    unittest.main()
