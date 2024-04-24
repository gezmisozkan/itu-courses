# Özkan Gezmis k12327230
from bayesian_net import BayesianNet

bn = BayesianNet([
    # TODO: construct the Bayesian network based on story on Moodle (A5 "Constructing a Bayesian Net": The Story)
    # TODO: you don't need to initialize the conditional probability tables

    ('Creatures', ''),
    ('Climate', ''),
    ('Curiosity', ''),
    ('Resources', 'Climate'),
    ('Exploration', 'Resources Creatures Curiosity'),
    ('Happy', '')

])


# TODO: visualize the result
# TODO: include your first name and matriculation number in the title

bn.draw("BN by Özkan Gezmis, k12327230")

