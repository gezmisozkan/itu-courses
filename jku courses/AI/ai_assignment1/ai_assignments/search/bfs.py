# Özkan Gezmis 12327230

import queue

from ..problem import Problem, Node
from .. datastructures.queue import Queue


# please ignore this
def get_solver_mapping():
    return dict(bfs=BFS)


class BFS(object):
    # TODO, exercise 1:
    # - implement Breadth First Search (BFS)
    # - use 'problem.get_start_node()' to get the node with the start state
    # - use 'problem.is_end(node)' to check whether 'node' is the node with the end state
    # - use a set() to store already visited nodes
    # - use the 'queue' datastructure that is already imported as the 'fringe'/ the 'frontier'
    # - use 'problem.successors(node)' to get a list of nodes containing successor states
    def solve(self, problem: Problem):
        current = problem.get_start_node()                      # the starting node
        visited_nodes = set()                                   # set of visited nodes
        fringe = queue.Queue()                                  # queue of the nodes to visit
        fringe.put(current)                                     # first element is the starting node

        while not problem.is_end(current):                      # as long as the current node is not the end
            if current not in visited_nodes:                    # if the node is visited it won't be visited again
                extended_nodes = problem.successors(current)  # get the successor nodes for the current node
                for node in extended_nodes:                     # adding extended_nodes to queue
                    fringe.put(node)
            visited_nodes.add(current)                          # current node is visited
            fringe.get()                                        # and it should be deleted
            current = fringe.queue[0]                           # new current is the first element of the queue

        return current
