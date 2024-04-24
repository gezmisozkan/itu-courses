# Özkan Gezmis 12327230
class NodeGroup:  # I put NodeGroup class here since I just use it in this file
    def __init__(self):
        self.a = None
        self.b = None
        self.c = None
        self.t0 = None
        self.t1 = None
        self.t2 = None
        self.t3 = None


def get_components(x, y, z):  # get_components function gets x,y,z nodes as inputs and returns a,b,c,t0,t1,t2,t3
    g = NodeGroup()  # we need a node group
    if z.right == y:
        if y.right == x:  # if this is true it means the form is "right-right"
            g.a = z
            g.b = y
            g.c = x
            g.t0 = z.left
            g.t1 = y.left
            g.t2 = x.left
            g.t3 = x.right
        elif y.left == x:  # if this is true it means the form is "right-left"
            g.a = z
            g.b = x
            g.c = y
            g.t0 = z.left
            g.t1 = x.left
            g.t2 = x.right
            g.t3 = y.right
    elif z.left == y:  # if this is true it means the form is "left-left"
        if y.left == x:
            g.a = x
            g.b = y
            g.c = z
            g.t0 = x.left
            g.t1 = x.right
            g.t2 = y.right
            g.t3 = z.right
        elif y.right == x:  # if this is true it means the form is "left-right"
            g.a = y
            g.b = x
            g.c = z
            g.t0 = y.left
            g.t1 = x.left
            g.t2 = x.right
            g.t3 = z.right
    return g  # function returns nodeGroup g
