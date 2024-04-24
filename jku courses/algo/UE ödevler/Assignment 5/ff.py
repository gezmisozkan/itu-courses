#Özkan Gezmis 12327230
# Ford-Fulkerson algorithm in Python
import queue
class Graph:

    def __init__(self, graph):
        self.graph = graph  # original graph
        self.residual_graph = [[cell for cell in row] for row in graph]  # cloned graph
        self.latest_augmenting_path = [[0 for _ in row] for row in graph]  # empty graph with same dimension as graph
        self.current_flow = [[0 for _ in row] for row in graph]  # empty graph with same dimension as graph

    def ff_step(self, source, sink):
        """
        Perform a single flow augmenting iteration from source to sink. Update the latest augmenting path, the residual
        graph and the current flow by the maximum possible amount, according to the path found by BFS.
        @param source the source's vertex id
        @param sink the sink's vertex id
        @return the amount by which the flow has increased.
        """
        # TODO
        parent = [-1] * len(self.graph[0])  # parent list, it will contain parent of each node, I initialized it with -1
        # since there is not a node with index -1

        if self.BFS(source, sink, parent): # BFS for path finding, if it is False, there is no path
            max_flow = 1e5  # maximum possible amount of flow
            g = sink
            while(g != source):  # start with sink and come back until source
                max_flow = min(max_flow, self.residual_graph[parent[g]][g])  # graph[parent[g]][g] is directed edge
                # max flow is minimum number along the path, but it is maximum amount that we can take
                g = parent[g]  # every step come closer to source

            s = sink

            def reset_to_0(the_array):  # the function fills matrix with 0
                for i, e in enumerate(the_array):
                    if isinstance(e, list):
                        reset_to_0(e)
                    else:
                        the_array[i] = 0

            reset_to_0(self.latest_augmenting_path)  # I need to reset matrix to add just the last path, I couldn't find any other way
            while s != source:  # update residual graph along the path
                p = parent[s]
                self.residual_graph[p][s] -= max_flow  # this edge is the correct path, we should decrease the amount of flow
                self.residual_graph[s][p] += max_flow  # this edge is back-edge, we should increase edge value

                self.latest_augmenting_path[p][s] = max_flow  # this matrix is initially full of zeros, we add lats path to it

                if self.graph[p][s] > 0:  # add flow to matrix if path from the original graph is positive
                    self.current_flow[p][s] += max_flow
                else:  # else that means path is actually back-edge so we need to decrease from current flow
                    self.current_flow[s][p] -= max_flow

                s = parent[s]  # every step come closer to source
            return max_flow  # returns max flow of the path
        return 0  # if return value is 0 next function will stop

    def ford_fulkerson(self, source, sink):
        """
        Execute the ford-fulkerson algorithm (i.e., repeated calls of ff_step())
        @param source the source's vertex id
        @param sink the sink's vertex id
        @return the max flow from source to sink
        """
        # TODO
        max_flow = self.ff_step(source, sink)
        total_flow = max_flow

        while True:  # runs algorithm until max flow is smaller than 1
            max_flow = self.ff_step(source, sink)
            if max_flow > 0:
                total_flow += max_flow  # calculates total flow
            else:
                break

        return total_flow  # returns total flow

    def BFS(self, s, t, parent):  # searches path and returns True if any path exist
        visited = [False] * (len(self.residual_graph[0]))  # list of the visited nodes whose length is the length of the graph matrix row
        q = queue.Queue()  # initialize queue for BFS with source
        q.put(s)
        visited[s] = True  # source node will be visited first

        while not q.empty():  # continue until end
            current = q.get()  # first element is popped
            current_row = self.residual_graph[current]  # row of the matrix whose index is current node
            for index, value in enumerate(current_row):  # enumerate function gives tuple, first element is index second is flow value
                if visited[index] is False and value > 0:  # if node is not visited and flow is not 0
                    q.put(index)  # add new node to queue
                    visited[index] = True  # mark node as visited
                    parent[index] = current  # the parent of the newly visited node is the current node
                    if index == t:  # if element is sink we can finish the loop and return True
                        return True

        return False  # if we didn't reach sink, that means there is no path between source and sink, and return False
