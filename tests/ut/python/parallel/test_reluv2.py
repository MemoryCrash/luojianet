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
# ============================================================================
import numpy as np
import luojianet_ms as ms
import luojianet_ms.context as context
from luojianet_ms import Tensor, Parameter
import luojianet_ms.nn as nn
from luojianet_ms.common.api import _cell_graph_executor
from luojianet_ms.nn import TrainOneStepCell, Momentum
from luojianet_ms.ops import operations as P


class Net(nn.Module):
    def __init__(self, mul_weight, strategy=None):
        super(Net, self).__init__()
        self.reluv2 = P.ReLUV2().shard(strategy)
        self.mul = P.Mul()
        self.weight = Parameter(mul_weight, "w1")

    def call(self, x):
        out = self.mul(x, self.weight)
        output, _ = self.reluv2(out)
        return output


_w1 = Tensor(np.ones([32, 16, 48, 64]), dtype=ms.float32)
_x = Tensor(np.ones([32, 16, 48, 64]), dtype=ms.float32)


def compile_net(net):
    context.set_context(mode=context.GRAPH_MODE)
    optimizer = Momentum(net.trainable_params(), learning_rate=0.1, momentum=0.9)
    train_net = TrainOneStepCell(net, optimizer)
    train_net.set_auto_parallel()
    train_net.set_train()
    _cell_graph_executor.compile(train_net, _x)
    context.reset_auto_parallel_context()


def test_reluv2():
    """
    Feature: distribute operator reluv2 in auto parallel.
    Description: mul-reluv2 net with strategy in semi auto parallel.
    Expectation: compile done without error.
    """
    context.set_auto_parallel_context(parallel_mode="semi_auto_parallel", device_num=8, global_rank=0)
    strategy = ((2, 1, 2, 2),)
    net = Net(_w1, strategy)
    compile_net(net)


def test_reluv2_no_full():
    """
    Feature: distribute operator reluv2 in auto parallel.
    Description: mul-reluv2 net with strategy in semi auto parallel, not fully shard.
    Expectation: compile done without error.
    """
    context.set_auto_parallel_context(parallel_mode="semi_auto_parallel", device_num=8, global_rank=0)
    strategy = ((2, 1, 2, 1),)
    net = Net(_w1, strategy)
    compile_net(net)


def test_reluv2_no_strategy():
    """
    Feature: distribute operator reluv2 in auto parallel.
    Description: mul-reluv2 net without strategy in semi auto parallel.
    Expectation: compile done without error.
    """
    context.set_auto_parallel_context(parallel_mode="semi_auto_parallel", device_num=8, global_rank=0)
    strategy = None
    net = Net(_w1, strategy)
    compile_net(net)


def test_reluv2_auto_parallel():
    context.set_auto_parallel_context(parallel_mode="auto_parallel", device_num=8, global_rank=0)
    net = Net(_w1)
    compile_net(net)
