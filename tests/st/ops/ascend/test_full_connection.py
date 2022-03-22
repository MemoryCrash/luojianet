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

import luojianet_ms.context as context
import luojianet_ms.nn as nn
from luojianet_ms.common.api import ms_function
from luojianet_ms.ops import operations as P

context.set_context(device_target="Ascend")


class Net(nn.Cell):
    def __init__(self):
        super(Net, self).__init__()
        self.matmul = P.MatMul(transpose_b=True)
        self.bias_add = P.BiasAdd()

    @ms_function
    def construct(self, x, w, b):
        return self.bias_add(self.matmul(x, w), b)

# def test_net():
#     x = np.random.randn(32, 2048).astype(np.float16)
#     w = np.random.randn(1001, 2048).astype(np.float16)
#     b = np.random.randn(1001).astype(np.float16)
#     FullConnection = Net()
#     output = FullConnection(Tensor(x), Tensor(w), Tensor(b))
#     print(output.asnumpy())
