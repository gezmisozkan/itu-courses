# Özkan Gezmis 12327230
import component
import avl_tree


def restructure_tree(x, y, z):  # function get x,y,z as inputs and returns the root of the restructured tree
    g = component.get_components(x, y, z)  # we must find components first
    g.b.parent = z.parent  # new root is be "b" so, to prevent the tree from being disconnected, the parent of z must
    # be the new parent of b
    if z.parent is not None:  # likewise, we need to establish the relationship between the new root b and the parent
        # of z
        if z.parent.left == z:
            z.parent.left = g.b
        elif z.parent.right == z:
            z.parent.right = g.b
    # I used set_left and set_right functions from AVLTree class

    # For all rotations the balanced tree form is same. For example a is left child of b, and c is right child of b
    # t0, t1 are children of a and t2,t3 are children of c
    avl_tree.AVLTree.set_left(avl_tree.AVLTree(), g.b, g.a)
    avl_tree.AVLTree.set_right(avl_tree.AVLTree(), g.b, g.c)
    avl_tree.AVLTree.set_left(avl_tree.AVLTree(), g.a, g.t0)
    avl_tree.AVLTree.set_right(avl_tree.AVLTree(), g.a, g.t1)
    avl_tree.AVLTree.set_left(avl_tree.AVLTree(), g.c, g.t2)
    avl_tree.AVLTree.set_right(avl_tree.AVLTree(), g.c, g.t3)

    return g.b  # root node of balanced tree
