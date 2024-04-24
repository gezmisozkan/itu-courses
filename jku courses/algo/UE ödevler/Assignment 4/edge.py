from vertex import Vertex

class Edge():
    def __init__(self, first: Vertex, second: Vertex, weight):
        self.first = first     # reference to a vertex
        self.second = second   # reference to a vertex
        self.weight = weight   # weight of the edge
