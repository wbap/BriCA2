import numpy as np
import chainer
from chainer import cuda
import chainer.functions as F
import chainer.links as L
from chainer import optimizers

import data
import net

mnist = data.load_mnist_data()
mnist['data'] = mnist['data'].astype(np.float32)
mnist['data'] /= 255
mnist['target'] = mnist['target'].astype(np.int32)

N_train = 60000
x_train, x_test = np.split(mnist['data'],   [N_train])
y_train, y_test = np.split(mnist['target'], [N_train])
N_test = y_test.size

class MNISTComponent(object):
    def __init__(self, batchsize):
        self.batchsize = batchsize
        self.batch = 0
        self.perm = np.random.permutation(N_train)

    def fire(self, inputs):
        i = self.batch
        j = i + self.batchsize
        outputs = {'x': x_train[self.perm[i:j]], 't': y_train[self.perm[i:j]]}
        self.batch += self.batchsize
        if self.batch >= N_train:
            self.batch %= N_train
            self.perm = np.random.permutation(N_train)
        return outputs

class PerceptronComponent(object):
    def __init__(self, n_input, n_output):
        self.model = net.Perceptron(n_input, n_output)
        self.optimizer = optimizers.Adam()
        self.optimizer.setup(self.model)

    def train(self, x, t):
        self.optimizer.zero_grads()
        y = self.model(x)
        loss = F.softmax_cross_entropy(y, t)
        loss.backward()
        self.optimizer.update()
        accuracy = F.accuracy(y, t)
        return loss, accuracy

    def fire(self, inputs):
        x = chainer.Variable(inputs['x'])
        t = chainer.Variable(inputs['t'])
        loss, accuracy = self.train(x, t)
        y = self.model(x)
        t = np.array(np.argmax(y.data, axis=1), dtype=np.int32)
        return {'y': y.data, 't': t, 'loss': np.array([loss.data], dtype=np.float32), 'accuracy': np.array([accuracy.data], dtype=np.float32)}

class AutoencoderComponent(object):
    def __init__(self, n_input, n_output):
        self.model = net.Autoencoder(n_input, n_output)
        self.optimizer = optimizers.Adam()
        self.optimizer.setup(self.model)

    def train(self, x):
        self.optimizer.zero_grads()
        y = self.model(x, train=True)
        loss = F.mean_squared_error(y, x)
        loss.backward()
        self.optimizer.update()
        return loss

    def fire(self, inputs):
        x = chainer.Variable(inputs['x'])
        t = chainer.Variable(inputs['t'])
        loss = self.train(x)
        y = self.model(x, train=False)
        h = self.model.encode(x)
        return {'y': y.data, 'h': h.data, 't': t.data, 'loss': np.array([loss.data], dtype=np.float32)}
