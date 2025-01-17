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


class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.maxpool = P.MaxPool(pad_mode="SAME", kernel_size=3, strides=2)

    @ms_function
    def call(self, x):
        output = self.maxpool(x)
        return output


def test_net():
    x = np.random.randn(32, 64, 112, 112).astype(np.float16)
    context.set_context(mode=context.GRAPH_MODE, device_target="Ascend")
    maxpool = Net()
    output = maxpool(Tensor(x))
    print(output.asnumpy())

    context.set_context(mode=context.PYNATIVE_MODE, device_target="Ascend")
    maxpool = Net()
    output = maxpool(Tensor(x))
    print(output.asnumpy())
