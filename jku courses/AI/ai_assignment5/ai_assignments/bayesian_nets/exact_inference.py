# Özkan Gezmis k12327230
from bayesian_net import BayesianNet

T, F = True, False


def build_network():

    # TODO: build network that is provided for you on Moodle: A5 Exact Inference, include probability distributions
    bn = BayesianNet([  # construction of Bayesian Network

        ('MusicPlaying', '', 0.1),
        ('Dancing', 'MusicPlaying', {T: 0.3, F: 0.05}),
        ('Singing', 'MusicPlaying', {T: 0.2, F: 0.03}),
        ('Vacation', '', 0.09),
        ('Relaxed', 'Vacation', {T: 0.85, F: 0.6}),
        ('Happy', 'Relaxed Singing', {(T,T): 0.7, (T,F): 0.65, (F,T): 0.1, (F,F): 0.01})
    ])

    return bn


if __name__ == '__main__':
    bn = build_network()
    # optional: visualize network to check whether the structure is correct

    # TODO: compute the answers to the probabilistic queries (provided on Moodle A5 Exact Inference)
    # TODO: print results
    # TODO: enter required numbers in Moodle
    # Hint: use bn.event_probability(event)

    bn.draw("BN by Özkan Gezmis, k12327230")

    # observations are below. I added Dancing even though it is conditionally independent of Singing
    observation1 = {"MusicPlaying": T, "Singing": T, "Happy": F, "Vacation": T, "Relaxed": T, "Dancing": T}
    observation2 = {"MusicPlaying": T, "Singing": T, "Happy": F, "Vacation": T, "Relaxed": T, "Dancing": F}
    observation3 = {"MusicPlaying": T, "Singing": F, "Happy": F, "Vacation": T, "Relaxed": T, "Dancing": T}
    observation4 = {"MusicPlaying": T, "Singing": F, "Happy": F, "Vacation": T, "Relaxed": T, "Dancing": F}

    # a1 + a2 is the answer to the question 2 on Moodle
    # a3 + a4 is the answer to the question 3 on Moodle
    a1 = bn.event_probability(observation1)
    a2 = bn.event_probability(observation2)
    a3 = bn.event_probability(observation3)
    a4 = bn.event_probability(observation4)

    alpha = 1 / (a1 + a2 + a3 + a4)
    q1 = a1 + a2
    q2 = a3 + a4
    # answer of the question 4 is alpha * (a1 + a2)
    # answer of the question 5 is alpha * (a3 + a4)

    print(alpha)     # q1
    print(q1)        # q2
    print(q2)        # q3
    print(alpha*q1)  # q4
    print(alpha*q2)  # q5
