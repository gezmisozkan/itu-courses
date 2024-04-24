from edge import Edge
from vertex import Vertex

class Graph():
    def __init__(self):
        self.vertices = []  # list of vertices in the graph
        self.edges = []  # list of edges in the graph

    def add_vertex(self, vertex_name):
        """
        Inserts a new vertex with the given name into the graph.
        Returns None if the graph already contains a vertex with the same name.

        :param vertex_name: The name of vertex to be inserted
        :return: The newly added vertex, or None if the vertex was already part of the graph
        """
        if self.find_vertex(vertex_name) is not None:
            return None
        new_vertex = Vertex(vertex_name)
        self.vertices.append(new_vertex)
        return new_vertex

    def add_edge(self, v1_name, v2_name, weight: int):
        """
        Inserts an edge between two vertices with the names v1_name and v2_name and returns the newly added edge.
        None is returned if the edge already existed, or if at least one of the vertices is not found in the graph.
        :param v1_name: name of vertex 1
        :param v2_name: name of vertex 2
        :param weight: weight of the edge
        :return: Returns None if the edge already exists or at least one of the two given vertices is not part of the
                 graph, otherwise returns the newly added edge.
        """

        v1 = self.find_vertex(v1_name)
        v2 = self.find_vertex(v2_name)
        if v1 is None or v2 is None or self.find_edge(v1_name, v2_name) is not None:
            return None

        new_edge = Edge(v1, v2, weight)
        self.edges.append(new_edge)
        return new_edge

    def find_vertex(self, vertex_name):
        """
        Returns the respective vertex for a given name, or None if no matching vertex is found.
        :param vertex_name: the name of the vertex to find
        :return: the found vertex, or None if no matching vertex has been found.
        """
        for vertex in self.vertices:
            if vertex.name == vertex_name:
                return vertex
        return None

    def find_edge(self, v1_name, v2_name):
        """
        Returns the edge if there is an edge between the vertices with the name v1_name and v2_name, otherwise None.
        :param v1_name: name (string) of vertex 1
        :param v2_name: name (string) of vertex 2
        :return: Returns the found edge or None if there is no edge.
        """
        for edge in self.edges:
            if (edge.first.name == v1_name and edge.second.name == v2_name) or \
                    (edge.first.name == v2_name and edge.second.name == v1_name):
                return edge

        return None

    def neighbors(self, vertex_name):
        """
        Returns a list of vertices which are adjacent to the vertex with name vertex_name.
        :param vertex_name: The name of the vertex to which adjacent vertices are searched.
        :return: list of vertices that are adjacent to the vertex with name vertex_name.
        """
        vertex = self.find_vertex(vertex_name)
        if vertex is None:
            return None
        neighbors = []
        for edge in self.edges:
            if edge.first == vertex or edge.second == vertex:
                neighbors.append(edge.second if edge.first == vertex else edge.first)
        return neighbors
