# Özkan Gezmis
# 12327230
import sys

from graph import Graph
from vertex import Vertex
from step import Step

class PathClass:  # I have to define this class otherwise it gives an error in the unit test
    def __init__(self, vertex: Vertex, distance: int):
        self.point = vertex
        self.covered_distance = distance

class JKUMap(Graph):
    # visitedNodes = []
    # distances = []

    def __init__(self):
        super().__init__()
        v_spar = self.add_vertex("Spar")
        v_lit = self.add_vertex("LIT")
        v_porter = self.add_vertex("Porter")
        v_open_lab = self.add_vertex("Open Lab")
        v_bank = self.add_vertex("Bank")
        v_khg = self.add_vertex("KHG")
        v_chat = self.add_vertex("Chat")
        v_parking = self.add_vertex("Parking")
        v_bella_casa = self.add_vertex("Bella Casa")
        v_lib = self.add_vertex("Library")
        v_lui = self.add_vertex("LUI")
        v_teichwerk = self.add_vertex("Teichwerk")
        v_sp1 = self.add_vertex("SP1")
        v_sp3 = self.add_vertex("SP3")
        v_castle = self.add_vertex("Castle")
        v_papaya = self.add_vertex("Papaya")
        v_jkh = self.add_vertex("JKH")

        self.add_edge(v_jkh.name, v_papaya.name, 80)
        self.add_edge(v_papaya.name, v_castle.name, 85)
        self.add_edge(v_sp3.name, v_sp1.name, 130)
        self.add_edge(v_sp1.name, v_lui.name, 175)
        self.add_edge(v_sp1.name, v_parking.name, 240)
        self.add_edge(v_parking.name, v_bella_casa.name, 145)
        self.add_edge(v_parking.name, v_khg.name, 190)
        self.add_edge(v_khg.name, v_bank.name, 150)
        self.add_edge(v_khg.name, v_spar.name, 165)
        self.add_edge(v_spar.name, v_lit.name, 50)
        self.add_edge(v_spar.name, v_porter.name, 103)
        self.add_edge(v_lit.name, v_porter.name, 80)
        self.add_edge(v_porter.name, v_open_lab.name, 70)
        self.add_edge(v_porter.name, v_bank.name, 100)
        self.add_edge(v_chat.name, v_bank.name, 115)
        self.add_edge(v_chat.name, v_lib.name, 160)
        self.add_edge(v_chat.name, v_lui.name, 240)
        self.add_edge(v_lui.name, v_teichwerk.name, 135)
        self.add_edge(v_lui.name, v_lib.name, 90)

    def get_shortest_path_from_to(self, from_vertex: Vertex, to_vertex: Vertex):
        """
        This method determines the shortest path between two POIs "from_vertex" and "to_vertex".
        It returns the list of intermediate steps of the route that have been found
        using the dijkstra algorithm.

        :param from_vertex: Start vertex
        :param to_vertex:   Destination vertex
        :return:
           The path, with all intermediate steps, returned as an list. This list
           sequentially contains each vertex along the shortest path, together with
           the already covered distance (see example on the assignment sheet).
           Returns None if there is no path between the two given vertices.
        :raises ValueError: If from_vertex or to_vertex is None, or if from_vertex equals to_vertex
        """
        if from_vertex is None:
            raise ValueError("from vertex cannot be None")
        elif to_vertex is None:
            raise ValueError("to vertex cannot be None")
        elif from_vertex == to_vertex:
            raise ValueError("from vertex and to vertex cannot be same")
        visited_set = set()  # set of visited vertices
        paths = dict()  # paths between starting vertex to other vertices
        distances = dict()  # all distances between starting vertex to other vertices
        for vertex in self.vertices:  # it assigns 10^7 as a distance to all vertices as infinity
            distances[vertex.name] = 1e7
            paths[vertex.name] = from_vertex.name + " 0"  # all paths start with from_vertex

        distances[from_vertex.name] = 0  # starting vertex's distance should be 0
        self._dijkstra(from_vertex, visited_set, distances, paths)  # call dijkstra function

        path_list = list()  # list that will be returned
        path_str = paths[to_vertex.name].split()  # in the dijkstra function paths are concatenated so we need to split it
        if len(path_str) == 2:  # that means it just have string "from_node.name 0"
            return None  # Thus final point cannot be reached from starting point
        for i in range(len(path_str)):  # iterates string and adds paths to the list
            if i % 2 == 0:  # in the string elements should be considered evenly i.e. paths are like that SP1 130 KHG 560
                path = PathClass(self.find_vertex(path_str[i]), int(path_str[i+1]))  # first element is vertex second is distance
                path_list.append(path)
        return path_list  # returns final list of paths

    def get_shortest_distances_from(self, from_vertex: Vertex):
        """
        This method determines the amount of "steps" needed on the shortest paths
        from a given "from" vertex to all other vertices.
        The number of steps (or -1 if no path exists) to each vertex is returned
        as a dictionary, using the vertex name as key and the distance as value.
        E.g., the "from" vertex has a step count of 0 to itself and 1 to all adjacent vertices.

        :param from_vertex: start vertex
        :return:
          A map containing the number of steps (or -1 if no path exists) on the
          shortest path to each vertex, using the vertex name as key and the distance as value.
        :raises ValueError: If from_vertex is None.
        """
        if from_vertex is None:
            raise ValueError("from vertex cannot be None")
        visited_set = set()  # set of visited vertices
        paths = dict()  # paths between starting vertex to other vertices
        distances = dict()  # all distances between starting vertex to other vertices
        for vertex in self.vertices:  # it assigns 10^7 as a distance to all vertices as infinity
            distances[vertex.name] = 1e7
            paths[vertex.name] = from_vertex.name + " 0"  # all paths start with from_vertex

        distances[from_vertex.name] = 0  # starting vertex's distance should be 0
        self._dijkstra(from_vertex, visited_set, distances, paths)  # call dijkstra function
        for key in distances:  # dijkstra function calculates distances between starting vertex to others and if there
            # isn't any way between starting node and any other node distance stay 1e7, but I need to change this to -1
            # for unit test
            if distances[key] == 1e7:
                distances[key] = -1
        return distances  # returns all distances

    # This method is not mandatory, but a recommendation by us
    def _dijkstra(self, cur: Vertex, visited_set: set, distances: dict, paths: dict[str, str]):
        """
        This method is expected to be called with correctly initialized data structures and recursively calls itself.

        :param cur: Current vertex being processed
        :param visited_set: Set which stores already visited vertices.
        :param distances: Dict (nVertices entries) which stores the min. distance to each vertex.
        :param paths: Dict (nVertices entries) which stores the shortest path to each vertex.
        """
        visited_set.add(cur.name)  # set of visited elements
        adjacent_nodes = self.neighbors(cur.name)  # gets all adjacent nodes
        for node in adjacent_nodes:  # traverse adjacent vertices and compare distances
            distance = self.find_edge(cur.name, node.name).weight  # find distance of edge
            if distances[cur.name] + distance < distances[node.name]:  # if new distance is smaller than older
                distances[node.name] = distances[cur.name] + distance  # assign smaller one
                paths[node.name] = paths[cur.name] + " " + node.name + " " + str(distances[node.name])  # add path as
                # "vertex name" "distance" starting path is "starting vertex" "0" and it adds new vertex and distance to it

        next_node = self.find_next_node(visited_set, distances)  # find next node with helper function
        if next_node is not None:  # if next node is None that means we visited all reachable vertices
            self._dijkstra(next_node, visited_set, distances, paths)  # recursive function call
        return paths

    def find_next_node(self, visited_set: set, distances_old: dict):  # this function finds smallest unvisited vertex
        distances = distances_old.copy()  # I did that to prevent deletion of original dictionary
        while distances:  # if distance is None we visited all nodes
            next_node = min(distances, key=distances.get)  # find smallest vertex
            if next_node in visited_set or distances[next_node] == 1e7:  # if it is visited we should delete it from
                # the dictionary. Also if distance is 1e7 we cannot reach that vertex, so it cannot be our next node
                distances.pop(next_node)
            else:
                return self.find_vertex(next_node)  # if it is none that means we visited all nodes if not it returns
                # smallest unvisited node, find_vertex is used to get vertex as a Vertex object
        return None  # if all reachable elements are visited it returns None



