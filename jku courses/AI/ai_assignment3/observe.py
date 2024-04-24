import ai_assignments
from ai_assignments.environment import Outcome
from ai_assignments.utils.visualization import plot_environment_and_policy
import argparse
import textwrap
import os


def main():
    parser = argparse.ArgumentParser(
        description=textwrap.dedent('''
        this script lets you view a training instance in JSON format,
        and will display the environment, the immediate rewards,
        the policy, and value functions learned.
        '''),
        epilog=textwrap.dedent('''
        example usage:

        $ python observe.py test/gridworld.json test/q_learning.outcome
        this will view the problem instance 'test/gridworld.json', and display
        the policy and value functions contained in  'test/q_learning.outcome'
        '''),
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument('training_instance_name', type=str)
    parser.add_argument('outcome_filename', nargs='?', default=None)
    parser.add_argument('--coords', default=False, action='store_true')
    parser.add_argument('--grid', default=False, action='store_true')
    args = parser.parse_args()

    environment = ai_assignments.load_problem_instance(args.training_instance_name)

    outcome = None
    if args.outcome_filename is not None:
        with open(args.outcome_filename, 'r') as fh:
            outcome = Outcome.from_json(fh.read())

    policy = None
    Q = None
    V = None
    if outcome is not None:
        if outcome.policy is not None:
            policy = outcome.policy

        if outcome.V is not None:
            V = outcome.V

        if outcome.Q is not None:
            Q = outcome.Q

    plot_environment_and_policy(
        environment,
        policy,
        V,
        Q,
        show_coordinates=args.coords,
        show_grid=args.grid
    )


if __name__ == '__main__':
    main()
