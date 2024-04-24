from ai_assignments.utils.visualization import plot_game_tree
from copy import deepcopy
import ai_assignments
import argparse
import textwrap
import os


def main():
    parser = argparse.ArgumentParser(
        description=textwrap.dedent('''
        '''),
        epilog=textwrap.dedent('''
        '''),
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument('game_instance_filename', type=str)
    parser.add_argument('move_sequence_filename', nargs='?', default=None)
    parser.add_argument('--show_possible', action='store_true', default=False)
    args = parser.parse_args()

    proto_game = ai_assignments.load_problem_instance(args.game_instance_filename)

    move_sequence = []
    name = 'only starting position'
    if args.move_sequence_filename is not None:
        move_sequence_filename = args.move_sequence_filename
        name = os.path.splitext(os.path.split(move_sequence_filename)[-1])[0]
        with open(move_sequence_filename, 'r') as fh:
            move_sequence_string = fh.read()
            if move_sequence_string == '':
                print('move sequence file {} is empty'.format(move_sequence_filename))
            else:
                move_sequence = list(map(eval, move_sequence_string.split("|")[0].split(';')))
                print('move_sequence', move_sequence)

    print('#' * 30)
    print('#' * 30)
    print('starting position')
    print(proto_game.get_start_node())
    print('#' * 30)
    print('#' * 30)

    game = deepcopy(proto_game)
    nodes = []
    current = game.get_start_node()
    nodes.append(current)
    for player, move in move_sequence:
        if args.show_possible:
            successors = game.successors(current)
            nodes.extend(successors)
            current = None
            for succ in successors:
                if succ.action == move:
                    current = succ
                    break
        else:
            current = game.successor(current, move)
            nodes.append(current)

    print('-' * 30)
    print('sequence of nodes')
    for node in nodes:
        print('-' * 30)
        print(node)
        terminal, winner = game.outcome(node)
        print('terminal {}, winner {}'.format(terminal, winner))

    plot_game_tree(
        name,
        game,
        nodes
    )


if __name__ == '__main__':
    main()
