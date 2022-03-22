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

import luojianet_ms.context as context
import luojianet_ms.nn as nn
from luojianet_ms import Tensor
from luojianet_ms.common import dtype as mstype
from luojianet_ms.ops import composite as C

context.set_context(mode=context.GRAPH_MODE, device_target="GPU")


class Net(nn.Cell):
    def __init__(self, shape, seed=0):
        super(Net, self).__init__()
        self.shape = shape
        self.seed = seed

    def construct(self, mean, stddev):
        return C.normal(self.shape, mean, stddev, self.seed)


def test_net_1D():
    seed = 10
    shape = (3, 2, 4)
    mean = 1.0
    stddev = 1.0
    net = Net(shape, seed)
    tmean, tstddev = Tensor(mean, mstype.float32), Tensor(stddev, mstype.float32)
    output = net(tmean, tstddev)
    assert output.shape == (3, 2, 4)


def test_net_ND():
    seed = 10
    shape = (3, 1, 2)
    mean = np.array([[[1], [2]], [[3], [4]], [[5], [6]]]).astype(np.float32)
    stddev = np.array([1.0]).astype(np.float32)
    net = Net(shape, seed)
    tmean, tstddev = Tensor(mean, mstype.float32), Tensor(stddev, mstype.float32)
    output = net(tmean, tstddev)
    assert output.shape == (3, 2, 2)
