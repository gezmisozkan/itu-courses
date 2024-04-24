from ai_assignments import register_adversarial_search_method
from .minimax import Minimax
from .alphabeta import AlphaBeta
from .drunk_player import DrunkPlayer

register_adversarial_search_method('minimax', Minimax)
register_adversarial_search_method('alphabeta', AlphaBeta)
register_adversarial_search_method('drunk_player', DrunkPlayer)
