import ai_assignments
import argparse
import textwrap
import hashlib
import time
import copy
import os


def main():
    parser = argparse.ArgumentParser(
        description=textwrap.dedent('''
        this script reads in a game instance in JSON format, and runs
        one or more adversarial search methods to compute the outcome of the game.
        it writes the outcome of the game, and the sequence of moves/plies that led
        to this outcome into a file
        '''),
        epilog=textwrap.dedent('''
        example usage:

        $ python play.py test/game.json minimax
        this loads the game instance stored in 'test/game.json' and plays
        the game using the 'minimax' algorithm. the sequence of moves/plies
        is written to the file 'test/minimax.path'
        '''),
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        'game_instance_name',
        type=str,
        help='the name of the file that stores the game instance'
    )
    parser.add_argument(
        'adversarial_search_methods',
        nargs='+',
        choices=ai_assignments.get_adversarial_search_methods(),
        help='''choose one or more game playing methods / adversarial search methods
        to compute the outcome of the game.

        sequences of moves/plies will be written to a text file in the same directory
        as the problem_instance with the name <method_name>.path.

        solution hashes will be written to a text file in the same directory
        as the problem_instance with the name <method_name>.hash'''
    )
    args = parser.parse_args()

    if not os.path.exists(args.game_instance_name):
        print('the game instance ({}) does not exist'.format(args.game_instance_name))
        exit()

    game_instance = ai_assignments.load_problem_instance(args.game_instance_name)
    game_instance_directory, _ = os.path.split(args.game_instance_name)

    for search_method_name in args.adversarial_search_methods:
        # the game instance is copied, so that each of the game playing / adversarial search
        # methods gets a fresh copy to play, and all counts are reset
        current_game_instance = copy.deepcopy(game_instance)
        search_method = ai_assignments.get_adversarial_search_method(search_method_name)()

        t_start = time.time()
        outcome_node = search_method.play(current_game_instance)
        t_end = time.time()
        t_diff = t_end - t_start
        move_sequence = ai_assignments.game.get_move_sequence(outcome_node)

        nodes_expanded = current_game_instance.get_number_of_expanded_nodes()
        move_sequence_as_str = ';'.join(map(str, move_sequence)) + "|" + str(nodes_expanded)
        move_sequence_filename = os.path.join(
            game_instance_directory,
            '{}.path'.format(search_method_name)
        )
        with open(move_sequence_filename, 'w') as fh:
            fh.write(move_sequence_as_str)

        move_sequence_hash_as_str = hashlib.sha256(move_sequence_as_str.encode('UTF-8')).hexdigest()
        move_sequence_hash_filename = os.path.join(
            game_instance_directory,
            '{}.hash'.format(search_method_name)
        )
        with open(move_sequence_hash_filename, 'w') as fh:
            fh.write(move_sequence_hash_as_str)

        # output some info, for convenience
        print('### adversarial search method: {:>8s} #####################'.format(
            search_method_name
        ))
        print('game took            {:4.2f} seconds'.format(t_diff))
        print('move sequence as str {}'.format(move_sequence_as_str))
        print('nodes expanded       {}'.format(nodes_expanded))
        print('move sequence length {}'.format(len(move_sequence)))
        print('move sequence hash   {}'.format(move_sequence_hash_as_str))


if __name__ == '__main__':
    main()
