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
import luojianet_ms
import luojianet_ms.context as context
import luojianet_ms.nn as nn
from luojianet_ms import Tensor
from luojianet_ms.ops import operations as P
from luojianet_ms.ops.composite import GradOperation

context.set_context(mode=context.GRAPH_MODE, device_target="Ascend")

class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.resize = P.ResizeBilinear((2, 4), False)

    def call(self, x):
        return self.resize(x)


class Grad(nn.Module):
    def __init__(self, network):
        super(Grad, self).__init__()
        self.grad = GradOperation(get_all=True, sens_param=True)
        self.network = network
        self.network.set_train()

    def call(self, x, y):
        return self.grad(self.network)(x, y)


def net_float16():
    tensor = Tensor([[[[1, 2, 3, 4, 5], [2, 4, 6, 4, 9]]]], luojianet_ms.float16)
    net = Net()
    output = net(tensor)
    return output


def test_net_grad():
    net = Grad(Net())
    x = Tensor([[[[1, 2, 3, 4, 5], [2, 4, 6, 4, 9]]]], luojianet_ms.float16)
    y = net_float16()
    dy = Tensor([[[[1, 2, 3, 4], [2, 4, 6, 4]]]], luojianet_ms.float16)
    dy = P.Cast()(dy, luojianet_ms.float32)
    dx = net(x, dy)
    print("forward input: ", x)
    print("forward output: ", y)
    print("backward input: ", dy)
    print("backward output: ", dx)

    y_expect = np.array([[[[1.0, 2.25, 3.5, 4.75],
                           [2.0, 4.5, 5.0, 7.75]]]])
    dx_expect = np.array([[[[1.0, 1.5, 2.0, 2.5, 3.0],
                            [2.0, 3.0, 4.0, 4.0, 3.0]]]])
    assert np.array_equal(y_expect, y.asnumpy())
    assert np.array_equal(dx_expect, dx[0].asnumpy())
