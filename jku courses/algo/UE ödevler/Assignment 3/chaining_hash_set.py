# Özkan Gezmiş 12327230

from chaining_hash_node import ChainingHashNode


class ChainingHashSet():
    def __init__(self, capacity=0):
        self.hash_table = [None] * capacity
        self.table_size = 0
        self.capacity = capacity

    def get_hash_code(self, key):
        """Hash function that calculates a hash code for a given key using the modulo division.
        :param key:
        		Key for which a hash code shall be calculated according to the length of the hash table.
        :return:
        		The calculated hash code for the given key.

        """
        hash_code = key % self.capacity  # table is the main linked list and capacity is the length of it
        return hash_code  # calculated hash code

    def get_hash_table(self):
        """(Required for testing only)
        :return the hash table.
        """
        return self.hash_table  # returns hash table

    def set_hash_table(self, table):
        """(Required for testing only) Set a given hash table..
        :param table: Given hash table which shall be used.

        !!!
        Since this method is needed for testing we decided to implement it.
        You do not need to change or add anything.
        !!!

        """
        self.hash_table = table
        self.capacity = len(table)
        self.table_size = 0
        for node in table:
            while node is not None:
                self.table_size += 1
                node = node.next

    def get_table_size(self):
        """returns the number of stored keys (keys must be unique!)."""
        return self.table_size  # returns numbers of elements

    def insert(self, key):
        """Inserts a key and returns True if it was successful. If there is already an entry with the
          same key, the new key will not be inserted and False is returned.
         :param key:
         		The key which shall be stored in the hash table.
         :return:
         		True if key could be inserted, or False if the key is already in the hash table.
         :raises:
         		a ValueError if any of the input parameters is None.
         """
        if key is None:  # if key is None we should raise error
            raise Exception("key cannot be None")

        hash_code = self.get_hash_code(key)  # calculates hash code
        hash_node = ChainingHashNode(key)  # new node
        target_node = self.hash_table[hash_code]  # target node whose next node will be hash_node
        if self.hash_table[hash_code] is None:  # if at position of hash_code is free we should insert new node here
            self.hash_table[hash_code] = hash_node
        else:
            if target_node.key == key:  # I need to add this line since I cannot control first node in the loop
                return False
            while target_node.next is not None:  # until end node we should continue
                if target_node.next.key == key:  # each key should be unique
                    return False
                target_node = target_node.next

            target_node.next = hash_node  # new node is the end node of list at position hash_code
        self.table_size += 1  # after insert operation size is increased by 1
        return True

    def contains(self, key):
        """Searches for a given key in the hash table.
         :param key:
         	    The key to be searched in the hash table.
         :return:
         	    True if the key is already stored, otherwise False.
         :raises:
         	    a ValueError if the key is None.
         """
        if key is None:  # if key is None we should raise error
            raise Exception("key cannot be None")

        hash_code = self.get_hash_code(key)  # calculates hash code
        target_node = self.hash_table[hash_code]  # target node is the node we want to find

        if target_node is None:  # it means there are no nodes at position hash_code
            return False
        else:
            while target_node is not None:  # continues until end node
                if target_node.key == key:  # if key equal to target node's value it means hash table contains key
                    return True
                target_node = target_node.next

        return False  # if key cannot be found in the list that means hash table doesn't contain key

    def remove(self, key):
        """Removes the key from the hash table and returns True on success, False otherwise.
        :param key:
        		The key to be removed from the hash table.
        :return:
        		True if the key was found and removed, False otherwise.
        :raises:
         	a ValueError if the key is None.
        """
        if key is None:  # if key is None we should raise error
            raise Exception("key cannot be None")

        hash_code = self.get_hash_code(key)  # calculates hash code
        target_node = self.hash_table[hash_code]  # target node is the node we want to find

        if target_node is None:  # it means there are no nodes at position hash_code
            return False
        else:
            if target_node.key == key:  # target node is the first element of the chain
                self.hash_table[hash_code] = target_node.next  # new head should be next one
                self.table_size -= 1  # after delete operation size is decreased by 1
                return True  # key is removed
            while target_node.next is not None:  # continues until end node
                if target_node.next.key == key:  # if key equal to target node's value it means hash table contains key
                    target_node.next = target_node.next.next  # target_node.next which is wanted node is removed
                    self.table_size -= 1  # after delete operation size is decreased by 1
                    return True  # key is removed
                else:
                    target_node = target_node.next  # if key is not equal we should continue

        return False  # if key cannot be found in the list that means hash table doesn't contain key

    def clear(self):
        """Removes all stored elements from the hash table by setting all nodes to None.
        """
        for i in range(len(self.hash_table)):  # it traverses all the list and deletes heads of the lists garbage
            # collector will handle rest
            self.hash_table[i] = None
        self.table_size = 0  # after clear table size is now 0

    def to_string(self):
        """Returns a string representation of the hash table (array indices and stored keys) in the format
            Idx_0 {Node, Node, ... }, Idx_1 {...}
            e.g.: 0 {13}, 1 {82, 92, 12}, 2 {2, 32}, """
        for i in range(len(self.hash_table)):  # iterates all the list
            print(i, end=" {")  # start with index and opening curly brace
            node = self.hash_table[i]
            while node is not None:  # print all keys
                if node.next is not None:
                    print(node.key, end=", ")
                    node = node.next
                else:  # to remove last unnecessary comma
                    print(node.key, end="")
                    node = node.next
            if i != len(self.hash_table) - 1:
                print("}, ", end="")  # end with closing curly brace
            else:
                print("}", end="")  # to remove last unnecessary comma
