# Copyright 2021, 2022 LuoJiaNET Research and Development Group, Wuhan University
# Copyright 2021, 2022 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import numpy as np

import luojianet_ms as ms
import luojianet_ms.nn as nn
from luojianet_ms import Tensor
from luojianet_ms import context
from luojianet_ms.common.api import _cell_graph_executor
from luojianet_ms.ops import composite as C
from luojianet_ms.ops import operations as P
from tests.ut.python.ops.test_math_ops import VirtualLoss


grad_all = C.GradOperation(get_all=True)

class NetWithLoss(nn.Module):
    def __init__(self, network):
        super(NetWithLoss, self).__init__()
        self.loss = VirtualLoss()
        self.network = network

    def call(self, x, y):
        predict = self.network(x, y)
        return self.loss(predict)

class GradWrap(nn.Module):
    def __init__(self, network):
        super(GradWrap, self).__init__()
        self.network = network

    def call(self, x, y):
        return grad_all(self.network)(x, y)

class Net(nn.Module):
    def __init__(self, strategy=None):
        super().__init__()
        self.prelu = P.PReLU().shard(strategy)

    def call(self, x, y):
        out = self.prelu(x, y)
        return out

def compile_net(net, x, y):
    net.set_auto_parallel()
    net.set_train()
    _cell_graph_executor.compile(net, x, y)

def common_train_compile(input1_shape, input2_shape, strategy=None):
    x = Tensor(np.random.rand(*input1_shape), dtype=ms.float32)
    w = Tensor(np.random.rand(*input2_shape), dtype=ms.float32)
    net = GradWrap(NetWithLoss(Net(strategy)))
    compile_net(net, x, w)
    context.reset_auto_parallel_context()

def test_prelu_parallel_success1():
    """
    Feature: distribute operator prelu in auto parallel.
    Description: prelu net with strategy in semi auto parallel.
    Expectation: compile done without error.
    """
    context.set_auto_parallel_context(device_num=8, global_rank=0)
    context.set_auto_parallel_context(parallel_mode="semi_auto_parallel")
    common_train_compile((4, 4, 32, 64), (4,), ((1, 1, 1, 1), (1,)))


def test_prelu_parallel_success2():
    """
    Feature: distribute operator prelu in auto parallel.
    Description: prelu net with strategy in semi auto parallel.
    Expectation: compile done without error.
    """
    context.set_auto_parallel_context(device_num=64, global_rank=0)
    context.set_auto_parallel_context(parallel_mode="semi_auto_parallel")
    common_train_compile((4, 4, 32, 64), (4,), ((2, 1, 4, 8), (1,)))

def test_prelu_parallel_success3():
    """
    Feature: distribute operator prelu in auto parallel.
    Description: prelu net with strategy in semi auto parallel.
    Expectation: compile done without error.
    """
    context.set_auto_parallel_context(device_num=64, global_rank=0)
    context.set_auto_parallel_context(parallel_mode="semi_auto_parallel")
    common_train_compile((4, 16, 32, 64), (16,), ((2, 4, 4, 2), (4,)))


def test_prelu_parallel_success4():
    """
    Feature: distribute operator prelu in auto parallel.
    Description: prelu net with strategy in semi auto parallel.
    Expectation: compile done without error.
    """
    context.set_auto_parallel_context(device_num=64, global_rank=0)
    context.set_auto_parallel_context(parallel_mode="semi_auto_parallel")
    common_train_compile((4, 16, 32, 64), (1,), ((2, 4, 4, 2), (1,)))

def test_matmul_prelu_parallel_success():
    """
    Feature: distribute operator prelu in auto parallel.
    Description: matmul-prelu net with strategy in semi auto parallel.
    Expectation: compile done without error.
    """
    class NetWithLoss3(nn.Module):
        def __init__(self, network):
            super(NetWithLoss3, self).__init__()
            self.loss = VirtualLoss()
            self.network = network

        def call(self, x, y, w):
            predict = self.network(x, y, w)
            return self.loss(predict)

    class GradWrap3(nn.Module):
        def __init__(self, network):
            super(GradWrap3, self).__init__()
            self.network = network

        def call(self, x, y, w):
            return grad_all(self.network)(x, y, w)

    class Net3(nn.Module):
        def __init__(self, strategy1, strategy2):
            super().__init__()
            self.matmul = P.MatMul().shard(strategy1)
            self.prelu = P.PReLU().shard(strategy2)

        def call(self, x, y, w):
            out = self.matmul(x, y)
            out = self.prelu(out, w)
            return out

    context.reset_auto_parallel_context()
    context.set_auto_parallel_context(device_num=64, global_rank=0)
    context.set_auto_parallel_context(parallel_mode="semi_auto_parallel")
    strategy1 = ((2, 4), (4, 2))
    strategy2 = ((32, 1), (1,))
    x = Tensor(np.random.rand(128, 64), dtype=ms.float32)
    y = Tensor(np.random.rand(64, 16), dtype=ms.float32)
    w = Tensor(np.random.rand(16), dtype=ms.float32)
    net = GradWrap3(NetWithLoss3(Net3(strategy1, strategy2)))
    net.set_auto_parallel()
    net.set_train()
    _cell_graph_executor.compile(net, x, y, w)
