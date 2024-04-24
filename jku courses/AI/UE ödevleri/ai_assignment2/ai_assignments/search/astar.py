# Özkan Gezmis 12327230
import math

import ai_assignments
import solve
from .. problem import Problem
from .. datastructures.priority_queue import PriorityQueue


# please ignore this
def get_solver_mapping():
    return dict(
        astar_ec=ASTAR_Euclidean,
        astar_mh=ASTAR_Manhattan
    )


class ASTAR(object):
    # TODO, Exercise 2:
    # implement A* search (ASTAR)
    # - use the provided PriorityQueue where appropriate
    # - to put items into the PriorityQueue, use 'pq.put(<priority>, <item>)'
    # - to get items out of the PriorityQueue, use 'pq.get()'
    # - use a 'set()' to store nodes that were already visited
    def solve(self, problem: Problem):
        goal = problem.get_end_node()                                 # goal node is the end node
        current = problem.get_start_node()                            # the starting node
        visited_nodes = set()                                         # set of visited nodes
        pq = PriorityQueue()                                          # priority queue of the nodes to visit
        pq.put(0, current)                                     # first element is the starting node
        while not problem.is_end(current):                            # as long as the current node is not the end
            if current not in visited_nodes:                          # if the node is visited it won't be visited again
                extended_nodes = problem.successors(current)          # get the successor nodes for the current node
                for node in extended_nodes:                           # adding extended_nodes to the priority queue
                    cost = self.heuristic(node, goal) + int(node.cost)# heuristic cost calculation for A*, the heuristic
                                                                      # function changes according to distance method
                    pq.put(int(cost), node)                           # according to their costs
            visited_nodes.add(current)                                # current node is visited
            current = pq.get()                                    # new current should be the first element of the queue
        return current                                                # returned node is the solution


# please note that in an ideal world, the heuristics should actually be part
# of the problem definition, as it assumes domain knowledge about the structure
# of the problem, and defines a distance to the goal state


# this is the ASTAR variant with the euclidean distance as a heuristic
# it is registered as a solver with the name 'astar_ec'
class ASTAR_Euclidean(ASTAR):
    def heuristic(self, current, goal):
        cy, cx = current.state
        gy, gx = goal.state
        return math.sqrt((cy - gy) ** 2 + (cx - gx) ** 2)


# this is the ASTAR variant with the manhattan distance as a heuristic
# it is registered as a solver with the name 'astar_mh'
class ASTAR_Manhattan(ASTAR):
    def heuristic(self, current, goal):
        cy, cx = current.state
        gy, gx = goal.state
        return math.fabs((cy - gy)) + math.fabs(cx - gx)
