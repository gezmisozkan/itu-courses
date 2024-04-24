import numpy as np

def entropy(labels):
    """returns the same as scipy.stats.entropy([positive, negative], base=2)"""
    n = len(labels)
    if n == 0:
        return 0.0
    positive = sum(labels) / n
    negative = 1 - positive
    if positive == 0 or negative == 0:
        return 0.0
    return -positive * np.log2(positive) - negative * np.log2(negative)

def unique(L):
    L = list(dict.fromkeys(L))
    return L