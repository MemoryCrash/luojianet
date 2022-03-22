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
from luojianet_ms.common.api import ms_function
from luojianet_ms.ops import operations as P

context.set_context(device_target="Ascend", mode=context.GRAPH_MODE, variable_memory_max_size="31GB")


class Net(nn.Cell):
    def __init__(self):
        super(Net, self).__init__()
        self.relu = P.ReLU()

    @ms_function
    def construct(self, x):
        return self.relu(x)


def test_net():
    # size (31GB/2/-512)s/ize(float32) 4160749440
    x = np.random.randn(16, 120, 2167057).astype(np.float32)
    relu = Net()
    output = relu(Tensor(x))
    expect = 1 * (x > 0) * x
    print(x)
    print(output.asnumpy())
    print(expect)
    assert (output.asnumpy() == expect).all()
