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

import luojianet_ms.context as context
import luojianet_ms.nn as nn
from luojianet_ms import Tensor, Parameter
from luojianet_ms.common.api import _cell_graph_executor
from luojianet_ms.communication.management import init
from luojianet_ms.nn import Dense
from luojianet_ms.nn import Momentum
from luojianet_ms.nn import TrainOneStepCell, WithLossCell
from luojianet_ms.ops import operations as P
from luojianet_ms.context import ParallelMode
from luojianet_ms.communication._comm_helper import GlobalComm

class Net(nn.Module):
    def __init__(self, input_channel, out_channel):
        super(Net, self).__init__()
        weight_init1 = np.ones([64, 128]).astype(np.float32)
        weight_init2 = np.ones([32, 64]).astype(np.float32)
        self.weight1 = Parameter(Tensor(weight_init1), "loss_weight1", layerwise_parallel=True)
        self.weight2 = Parameter(Tensor(weight_init2), "loss_weight2", layerwise_parallel=True)
        self.fc = P.MatMul(transpose_b=True)
        self.dense = Dense(input_channel, out_channel)

    def call(self, x):
        x = self.dense(x)
        x = self.fc(x, self.weight1)
        x = self.fc(x, self.weight2)
        return x


def test_dense_gen_graph():
    context.set_context(mode=context.GRAPH_MODE)
    context.reset_auto_parallel_context()
    context.set_auto_parallel_context(parallel_mode=ParallelMode.HYBRID_PARALLEL, gradients_mean=True, device_num=8)
    GlobalComm.CHECK_ENVS = False
    init()
    GlobalComm.CHECK_ENVS = True
    network = Net(512, 128)

    loss_fn = nn.SoftmaxCrossEntropyWithLogits()
    optimizer = Momentum(filter(lambda x: x.requires_grad, network.get_parameters()),
                         learning_rate=0.1,
                         momentum=0.9)
    network = WithLossCell(network, loss_fn)
    network = TrainOneStepCell(network, optimizer)

    predict = Tensor(np.ones([64, 512]).astype(np.float32) * 0.01)
    label = Tensor(np.zeros([64, 32]).astype(np.float32))
    network.set_auto_parallel()
    _cell_graph_executor.compile(network, predict, label)
