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

context.set_context(mode=context.GRAPH_MODE, device_target="Ascend")


class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.matmul = P.MatMul()
        self.transpose = P.Transpose()
        self.reshape = P.Reshape()
        self.bias_add = P.BiasAdd()

    def call(self, x, y, z):
        res = self.matmul(x, y)
        res = self.bias_add(res, z)
        res = self.reshape(res, (24, 512, 16, 64))
        res = self.transpose(res, (0, 2, 1, 3))
        return res


def test_net():
    x = Tensor(np.ones(shape=[12288, 1024]), luojianet_ms.float16)
    y = Tensor(np.ones(shape=[1024, 1024]), luojianet_ms.float16)
    z = Tensor(np.ones(shape=[1024]), luojianet_ms.float16)
    net = Net()
    output = net(x, y, z)
    print("result", output.asnumpy())


if __name__ == "__main__":
    test_net()
