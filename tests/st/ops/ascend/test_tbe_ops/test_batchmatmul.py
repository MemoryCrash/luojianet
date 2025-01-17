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

from luojianet_ms import context
from luojianet_ms.common.tensor import Tensor
from luojianet_ms.nn import Module
from luojianet_ms.ops import operations as P
from luojianet_ms.train.model import Model

context.set_context(mode=context.GRAPH_MODE, device_target="Ascend")


class Net(Module):
    def __init__(self):
        super(Net, self).__init__()
        self.batchmatmul = P.BatchMatMul()

    def call(self, inputa, inputb):
        x = self.batchmatmul(inputa, inputb)
        return x


def tf_me_batchmatmul(inputa, inputb):
    net = Net()
    net.set_train()
    model = Model(net)
    model.predict(Tensor(inputa), Tensor(inputb))


def test_batchmatmul_normal_shape1():
    inputa = np.random.randn(128, 16, 128).astype(np.float32)
    inputb = np.random.randn(128, 128, 64).astype(np.float32)
    tf_me_batchmatmul(Tensor(inputa), Tensor(inputb))


def test_batchmatmul_normal_shape2():
    inputa = np.random.randn(1, 16, 128, 128).astype(np.float32)
    inputb = np.random.randn(1, 16, 128, 64).astype(np.float32)
    tf_me_batchmatmul(inputa, inputb)
