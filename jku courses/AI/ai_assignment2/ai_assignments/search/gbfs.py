# Özkan Gezmis 12327230
import math

import ai_assignments
import solve
from .. problem import Problem
from .. datastructures.priority_queue import PriorityQueue


# please ignore this
def get_solver_mapping():
    return dict(
        gbfs_mh=GBFS_Manhattan,
        gbfs_ch=GBFS_Chebyshev
    )


class GBFS(object):
    # TODO, Exercise 2:
    # - implement Greedy Best First Search (GBFS)
    # - use the provided PriorityQueue where appropriate
    # - to put items into the PriorityQueue, use 'pq.put(<priority>, <item>)'
    # - to get items out of the PriorityQueue, use 'pq.get()'
    # - use a 'set()' to store nodes that were already visited
    def solve(self, problem: Problem):
        goal = problem.get_end_node()                            # goal node is the end node
        current = problem.get_start_node()                       # the starting node
        visited_nodes = set()                                    # set of visited nodes
        pq = PriorityQueue()                                     # priority queue of the nodes to visit
        pq.put(0, current)                                # first element is the starting node
        while not problem.is_end(current):                       # as long as the current node is not the end
            if current not in visited_nodes:                     # if the node is visited it won't be visited again
                extended_nodes = problem.successors(current)     # get the successor nodes for the current node
                for node in extended_nodes:                      # adding extended_nodes to the priority queue
                    cost = self.heuristic(node,goal)             # heuristic cost calculation for GBFS, the heuristic
                                                                 # function changes according to distance method
                    pq.put(int(cost), node)                      # according to their costs
            visited_nodes.add(current)                           # current node is visited
            current = pq.get()                                   # new current should be the first element of the queue
        return current                                           # returned node is the solution


# please note that in an ideal world, the heuristics should actually be part
# of the problem definition, as it assumes domain knowledge about the structure
# of the problem, and defines a distance to the goal state


# this is the GBFS variant with the manhattan distance as a heuristic
# it is registered as a solver with the name 'gbfs_mh'
class GBFS_Manhattan(GBFS):
    def heuristic(self, current, goal):
        cy, cx = current.state
        gy, gx = goal.state
        return math.fabs((cy - gy)) + math.fabs(cx - gx)


# this is the GBFS variant with the chebyshev distance as a heuristic
# it is registered as a solver with the name 'gbfs_ch'
class GBFS_Chebyshev(GBFS):
    def heuristic(self, current, goal):
        # TODO, Exercise 2:
        # implement Chebyshev distance (see slides)
        # - get x- and y-coordinates for current and goal node (see Manhattan distance)
        # - compute Chebyshev distances based on formula in slides: max_i(|a_i - b_i|)
        # return Chebyshev distance
        cy, cx = current.state
        gy, gx = goal.state
        d1 = math.fabs((cy-gy))             # compute first distance
        d2 = math.fabs((cx-gx))             # compute second distance
        return max(d1,d2)                   # return max
