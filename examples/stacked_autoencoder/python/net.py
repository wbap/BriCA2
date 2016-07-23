import numpy as np
import chainer
import chainer.functions as F

class Perceptron(chainer.Chain):
    def __init__(self, n_input, n_output):
        super(Perceptron, self).__init__(
            transform=F.Linear(n_input, n_output)
        )

    def __call__(self, x):
        y = F.sigmoid(self.transform(x))
        return y

class Autoencoder(chainer.Chain):
    def __init__(self, n_input, n_output):
        super(Autoencoder, self).__init__(
            encode=F.Linear(n_input, n_output),
            decode=F.Linear(n_output, n_input),
        )

    def __call__(self, x, train=True):
        h = F.sigmoid(self.encode(F.dropout(x, train=train)))
        y = F.sigmoid(self.decode(F.dropout(h, train=train)))
        return y
