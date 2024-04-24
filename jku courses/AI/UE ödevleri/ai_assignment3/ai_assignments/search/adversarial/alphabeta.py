# Özkan Gezmis 12327230
from ...game import Game


class AlphaBeta():
    def play(self, game: Game):
        start = game.get_start_node()

        alpha = float('-Inf')
        beta = float('Inf')

        # 'game.get_max_player()' asks the game how it identifies the MAX player internally.
        # this is just for the sake of generality, so games are free to encode
        # player's identities however they want.
        # (it just so happens to be '1' for MAX, and '-1' for MIN most of the times)
        value, terminal_node = self.alphabeta(game, start, alpha, beta, game.get_max_player())
        return terminal_node

    def alphabeta(self, game, node, alpha, beta, max_player):
        # here we check if the current node 'node' is a terminal node
        terminal, winner = game.outcome(node)

        # if it is a terminal node, determine who won, and return
        # a) the value (-1, 0, 1)
        # b) the terminal node itself, to determine the path of moves/plies
        #    that led to this terminal node
        if terminal:
            if winner is None:
                return 0, node
            elif winner == max_player:
                return 1, node
            else:
                return -1, node

        # TODO, Exercise 3: implement the minimax-with-alpha-beta-pruning algorithm
        # recursively here. the structure is almost the same as for minimax

        if node.player == max_player:

            # this is how you get minus infinity as a 'value' (smaller than all other numbers)
            best_value = float('-Inf')
            best_node = None

            expanded_nodes = game.successors(node)  # list of expanded nodes
            for s in expanded_nodes:  # traversing tree with dfs
                s.player = -1  # next round, player is min_player
                # recursive function that returns winner of the game and leaf node with inputs node and alpha and beta
                returned_value, returned_node = self.alphabeta(game, s, alpha, beta, max_player)

                if returned_value > best_value:  # if returned value is greater than our best_value we should update
                    # best_value to returned_node since out purpose is find the highest value for max_player
                    # it should be strictly greater since if they are equal we want to return leftmost node
                    best_value, best_node = returned_value, returned_node

                # beta is the best value found so far for MIN
                # if best_value >= beta we can prune the rest of the nodes
                if best_value >= beta:
                    return best_value, best_node
                # we should update alpha
                alpha = max(alpha, best_value)
            return best_value, best_node
        else:

            # this is how you get plus infinity as a 'value' (larger than all other numbers)
            best_value = float('Inf')
            best_node = None

            expanded_nodes = game.successors(node)  # list of expanded nodes
            for s in expanded_nodes:  # traversing tree with dfs
                s.player = 1  # next round, player is max_player
                # recursive function that returns winner of the game and leaf node with inputs node and alpha and beta
                returned_value, returned_node = self.alphabeta(game, s, alpha, beta, max_player)

                if returned_value < best_value:  # if returned value is lower than our best_value we should update
                    # best_value to returned_node since out purpose is find the lowest value for min_player
                    # it should be strictly greater since if they are equal we want to return leftmost node
                    best_value, best_node = returned_value, returned_node

                # alpha is the best value found so far for MAX
                # if best_value <= alpha we can prune the rest of the nodes
                if best_value <= alpha:
                    return best_value, best_node
                # we should update beta
                beta = min(beta, best_value)
            return best_value, best_node

        # this is just here so that no error is thrown with the stub-implementation
        # you can delete this, when you start implementing
        # return None, None
