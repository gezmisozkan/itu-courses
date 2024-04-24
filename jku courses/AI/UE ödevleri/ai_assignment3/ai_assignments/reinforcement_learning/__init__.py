from .. import register_reinforcement_learning_method
from . q_learning import QLearning

register_reinforcement_learning_method('q_learning', QLearning)
