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
        this script reads in a training instance in JSON format, and runs
        one or more reinforcement learning methods to compute an optimal
        deterministic policy. the learned optimal policy is written into a file
        in the same directory as the training instance.
        '''),
        epilog=textwrap.dedent('''
        example usage:

        $ python train.py test/gridworld.json q_learning
        this loads the training instance stored in 'test/gridworld.json' and learns
        an optimal policy using the 'q_learning' algorithm. the optimal policy
        is written to the file 'test/q_learning.policy'.
        '''),
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        'training_instance_name',
        type=str,
        help='the name of the file that stores the training instance'
    )
    parser.add_argument(
        'reinforcement_learning_methods',
        nargs='+',
        choices=ai_assignments.get_reinforcement_learning_methods(),
        help='''choose one or more reinforcement learning methods
        to learn an optimal policy.

        the optimal policy will be written into a file in the same directory
        as the training_instance with the name <method_name>.policy.

        solution hashes will be written to a text file in the same directory
        as the training_instance with the name <method_name>.hash'''
    )
    args = parser.parse_args()

    if not os.path.exists(args.training_instance_name):
        print('the training instance ({}) does not exist'.format(args.training_instance_name))
        exit()

    training_instance = ai_assignments.load_problem_instance(args.training_instance_name)
    training_instance_directory, _ = os.path.split(args.training_instance_name)

    for rl_method_name in args.reinforcement_learning_methods:
        # the training instance is copied, so that each of the training
        # methods gets a fresh copy to train on, and all counts are reset
        current_training_instance = copy.deepcopy(training_instance)
        rl_method = ai_assignments.get_reinforcement_learning_method(rl_method_name)()

        t_start = time.time()
        outcome = rl_method.train(current_training_instance)
        t_end = time.time()
        t_diff = t_end - t_start

        outcome_filename = os.path.join(
            training_instance_directory,
            '{}.outcome'.format(rl_method_name)
        )
        with open(outcome_filename, 'w') as fh:
            fh.write(outcome.to_json())

        flat_policy = ai_assignments.environment.get_flat_policy(
            current_training_instance,
            outcome.policy
        )
        flat_policy_as_str = ','.join(map(str, flat_policy))
        flat_policy_hash_as_str = hashlib.sha256(flat_policy_as_str.encode('UTF-8')).hexdigest()
        flat_policy_hash_filename = os.path.join(
            training_instance_directory,
            '{}.hash'.format(rl_method_name)
        )
        with open(flat_policy_hash_filename, 'w') as fh:
            fh.write(flat_policy_hash_as_str)

        # output some info, for convenience
        print(f'### reinforcement learning method: {rl_method_name:>8s} #####################')
        print(f'training took        {t_diff:4.2f} seconds')
        print(f'nr of episodes       {outcome.get_n_episodes()}')
        print(f'flat_policy hash     {flat_policy_hash_as_str}')


if __name__ == '__main__':
    main()
