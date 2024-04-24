from ... game import Game
from ... utils.visualization import plot_game_tree
import random


class DrunkPlayer():
    def play(self, game: Game):
        # this list holds all the nodes that will be rendered
        self.visualize_nodes = []

        # this dict holds (key, value) pairs that will show
        # up as text on the node with id(node)
        self.annotations = dict()

        # change this seed number for the PRNG,
        # if you want to see different trees
        random.seed(43)
        node = self.random_walk(game, game.get_start_node())

        plot_game_tree(
            title='A random game tree',
            game=game,
            nodes=self.visualize_nodes,
            annotations=self.annotations
        )

        return node

    def random_walk(self, game, node):
        # we'll record all the nodes in the list that will be visualized
        self.visualize_nodes.append(node)

        # we will annotate nodes with a varying number of texts 'r<i>' and a random number
        annotations = dict()
        for r in range(random.randint(1, 4)):
            annotations['r{}'.format(r)] = random.randint(0, 9)
        self.annotations[id(node)] = annotations

        # we do not care who wins at all... drunk players are just like that: careless.
        terminal, _ = game.outcome(node)
        if terminal:
            return node

        # check what's possible if we're in this node
        successors = game.successors(node)

        # there will be at least 1 element in the successor list,
        # because terminal nodes were excluded above
        n_choices = random.randint(1, len(successors))

        # shuffle the order of the successor list
        random.shuffle(successors)

        # expand the first (random) <n_choices> successor nodes
        for i in range(n_choices):
            random_successor = successors[i]
            random_terminal_node = self.random_walk(game, random_successor)

        # return the right-most random_terminal_node, because we jus' don't care.
        return random_terminal_node
