# Özkan Gezmis 12327230

from .. problem import Problem
from .. datastructures.priority_queue import PriorityQueue


def get_solver_mapping():
    return dict(ucs=UCS)


class UCS(object):
    # TODO, excercise 2:
    # - implement Uniform Cost Search (UCS), a variant of Dijkstra's Graph Search
    # - use the provided PriorityQueue where appropriate
    # - to put items into the PriorityQueue, use 'pq.put(<priority>, <item>)'
    # - to get items out of the PriorityQueue, use 'pq.get()'
    # - store visited nodes in a 'set()'
    def solve(self, problem: Problem):
        current = problem.get_start_node()                      # the starting node
        visited_nodes = set()                                   # set of visited nodes
        fringe = PriorityQueue()                                # priority queue of the nodes to visit
        fringe.put(0,current)                            # first element is the starting node

        while not problem.is_end(current):                      # as long as the current node is not the end
            if current not in visited_nodes:                    # if the node is visited it won't be visited again
                extended_nodes = problem.successors(current)        # get the successor nodes for the current node
                for node in extended_nodes:                     # adding extended_nodes to the priority queue
                    fringe.put(int(node.cost), node)            # according to their costs
            visited_nodes.add(current)                          # current node is visited
            # fringe.get()                                        # and it should be deleted
            current = fringe.get()                              # new current should be the first element of the queue
            # fringe.put(0,current)                        # I deleted it from the queue and added the first place
                                                                # to get the element
        return current
