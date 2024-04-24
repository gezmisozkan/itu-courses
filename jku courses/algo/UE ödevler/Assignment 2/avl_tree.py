# Özkan Gezmis 12327230
from avl_node import AVLNode
import math
import restructure


class AVLTree:
    def __init__(self):
        self.root = None
        self.size = 0
        self.to_restruct = None

    def get_tree_root(self):
        """
        Method to get the root node of the AVLTree
        :return AVLNode -- the root node of the AVL tree
        """
        # TODO
        return self.root  # function returns root of the tree

    def get_tree_height(self):
        """Retrieves tree height.
        :return -1 in case of empty tree, current tree height otherwise.
        """
        # TODO
        if self.root is None:  # if tree is empty function returns -1
            return -1
        else:
            return self.root.height  # otherwise it returns height of the root

    def get_tree_size(self):
        """Return number of key/value pairs in the tree.
        :return Number of key/value pairs.
        """

        # TODO
        def calc_size(node):  # recursive function that calculates tree size
            if node is None:  # if node is None size is 0
                return 0
            else:  # else it returns size of left subtree + size of right subtree and we should add 1
                # since we didn't include root
                return calc_size(node.left) + calc_size(node.right) + 1

        return calc_size(self.root)  # for the size of tree we can use this function with input root

    def get_node_height(self, node):  # calculates node's height
        if node is None:  # if node is Null height should be -1
            return -1
        else:  # height of the node is maximum height of left subtree and right subtree + 1
            return max(self.get_node_height(node.left), self.get_node_height(node.right)) + 1

    def update_heights(self, node):  # updates node's heights
        if node is not None:
            current = node
            while current is not None:  # until root update all the nodes' heights
                new_height = self.get_node_height(current)  # we should recalculate node's height
                current.height = new_height
                current = current.parent  # new node should be parent of the current node
        return None  # function only updates heights it doesn't need to return something

    def is_balanced(self, node):  # return True if the subtree is balanced
        if node is not None:  # if the height difference between sibling nodes is greater than or equal to 2 it means
            # tree is unbalanced
            if math.fabs(self.get_node_height(node.left) - self.get_node_height(node.right)) >= 2:
                return False
            else:
                return True
        else:  # if node is None it cannot be unbalanced
            return True

    def balance_tree(self, node):  # function for balancing tree
        current = node
        if current.parent is not None:
            while current.parent.parent is not None:  # grandparent of current node should not be None
                if not self.is_balanced(current.parent.parent):  # check if tree is balanced or not
                    x = current  # x is the current node
                    y = current.parent  # x,y and z are always in the same order
                    z = current.parent.parent
                    result = restructure.restructure_tree(current, current.parent, current.parent.parent)  # we give x,
                    # y,z as inputs to the function, and it returns balanced version of the tree

                    if result.parent is None:  # if result.parent is none, it means it should be new root because
                        # function returns root of the restructured tree
                        self.root = result
                    self.update_heights(x)  # after rotation, we should update heights for all nodes because it changes
                    self.update_heights(y)
                    self.update_heights(z)
                else:  # We need to check whether the tree is balanced all the way to the root, since just because
                    # one subtree is balanced does not mean that the entire tree is balanced.
                    current = current.parent
        return None  # When the function finishes the tree becomes balanced

    def find_by_key(self, key):
        """Returns value of node with given key.
        :param key: Key to search.
        :return Corresponding value if key was found, None otherwise.
        :raises ValueError if the key is None
        """
        if key is None:
            raise ValueError("Cannot search for null key!")
        current = self.root
        while current is not None:
            if current.key == key:
                return current.value
            elif current.key < key:
                current = current.right
            else:
                current = current.left

        return None

    def insert(self, key, value):
        """Inserts a new node into AVL tree.
        :param key: Key of the new node.
        :param value: Data of the new node. Must not be None. Nodes with the same key
        are not allowed. In this case False is returned. None-Keys and None-Values are
        not allowed. In this case an error is raised.
        :return True if the insert was successful, False otherwise.
        :raises ValueError if the key or value is None.
        """
        if key is None:
            raise ValueError("Null keys are not allowed!")
        node = AVLNode(key, value)  # node that will be added to the tree
        if self.root is None:
            self.root = node
        else:
            current = self.root
            while True:
                if current.key == key:
                    return False
                elif current.key < key:
                    if current.right is not None:
                        current = current.right
                    else:
                        self.set_right(current, node)
                        break
                else:
                    if current.left is not None:
                        current = current.left
                    else:
                        self.set_left(current, node)
                        # self.set_left(current, AVLNode(key, value))
                        break
        self.size += 1
        # TODO update heights, check AVL integrity, restructure if needed
        self.update_heights(node)  # update node's heights until root
        self.balance_tree(node)  # restructure the tree if needed
        return True

    def remove_by_key(self, key):
        """Removes node with given key.
        :param key: Key of node to remove.
        :return True If node was found and deleted, False otherwise.
        @raises ValueError if the key is None.
        """
        if key is None:
            raise ValueError("Null key is not allowed!")

        parent = None
        current = self.root
        new_sub_root = None

        while not (current is None):
            if current.key == key:
                if parent is None:
                    self.root = self._remove_bst(current)
                    if self.root is not None:
                        self.root.parent = None
                elif parent.left == current:
                    new_sub_root = self._remove_bst(current)
                    self.set_left(parent, new_sub_root)
                elif parent.right == current:
                    new_sub_root = self._remove_bst(current)
                    self.set_right(parent, new_sub_root)
                else:
                    raise ValueError()

                self.size -= 1
                # to_restruct is the node from which the search for the first unbalanced node is started
                if self.to_restruct is not None:
                    # todo update heights
                    self.update_heights(self.to_restruct)  # we should update to_restruct's height
                    while self.to_restruct is not None:  # until tree become balanced it should continue
                        # todo restructure tree until it is balanced
                        if not self.is_balanced(self.to_restruct): # we should find first unbalanced node z
                            z = self.to_restruct
                            y = AVLNode()
                            x = AVLNode()
                            if self.get_node_height(z.left) >= self.get_node_height(z.right):
                                y = z.left  # Put y on child of z with the greatest height
                            else:
                                y = z.right
                            if self.get_node_height(y.left) >= self.get_node_height(y.right):
                                x = y.left  # Put x on child of y with the greatest height
                            else:
                                x = y.right
                            sub_root = restructure.restructure_tree(x, y, z)  # we should restruct the subtree
                            self.update_heights(x)  # after rotation, we should update heights for all nodes
                            self.update_heights(y)
                            self.update_heights(z)
                            self.to_restruct = sub_root.parent  # we should continue the process so new to_restruct node
                            # becomes parent of old one
                        else:
                            self.to_restruct = self.to_restruct.parent  # if node isn't unbalanced we should continue
                return True
            else:
                parent = current
                if current.key > key:
                    current = current.left
                else:
                    current = current.right

        return False

    # auxiliary functions

    def _remove_bst(self, old_sub_root):
        new_sub_root = None
        if old_sub_root.left is None and old_sub_root.right is None:
            new_sub_root = None
            self.to_restruct = old_sub_root.parent
        elif old_sub_root.left is None:
            new_sub_root = old_sub_root.right
            self.to_restruct = new_sub_root
        elif old_sub_root.right is None:
            new_sub_root = old_sub_root.left
            self.to_restruct = new_sub_root
        elif old_sub_root.left.right is None:
            new_sub_root = old_sub_root.left
            self.set_right(new_sub_root, old_sub_root.right)
            self.to_restruct = new_sub_root
        elif old_sub_root.right.left is None:
            new_sub_root = old_sub_root.right
            self.set_left(new_sub_root, old_sub_root.left)
            self.to_restruct = new_sub_root
        else:
            new_sub_root = old_sub_root.left
            while new_sub_root.right is not None:
                new_sub_root = new_sub_root.right
            predecessor_p = new_sub_root.parent
            self.set_right(predecessor_p, new_sub_root.left)
            self.set_right(new_sub_root, old_sub_root.right)
            self.set_left(new_sub_root, old_sub_root.left)
            self.to_restruct = predecessor_p

        return new_sub_root

    def set_left(self, parent, child):
        parent.left = child
        if child is not None:
            child.parent = parent

    def set_right(self, parent, child):
        parent.right = child
        if child is not None:
            child.parent = parent
