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

"""test hccl allreduce with 8p"""

import os
import numpy as np
import luojianet_ms.nn as nn
from luojianet_ms import Tensor
from luojianet_ms import dtype as mstype
from luojianet_ms.ops import operations as P
from luojianet_ms.communication.management import init

np.random.seed(1)
os.environ['GRAPH_OP_RUN'] = str(1)
os.environ['HCCL_WHITELIST_DISABLE'] = str(1)
init()

class AllReduceNet(nn.Module):
    def __init__(self):
        super(AllReduceNet, self).__init__()
        self.mul = P.Mul()
        self.all_reduce = P.AllReduce()
        self.add = P.Add()
        self.y1 = Tensor(np.array([[2, 2, 2, 2], [2, 2, 2, 2], [2, 2, 2, 2]])).astype(np.float32)
        self.y2 = Tensor(np.array([[-16, -16, -16, -16], [-16, -16, -16, -16], \
                                   [-16, -16, -16, -16]])).astype(np.float32)

    def call(self, x):
        x = self.mul(x, 2)
        z = self.add(x, self.y1)
        z = self.all_reduce(z)
        out = self.add(z, self.y2)
        out = self.all_reduce(out)
        out = self.mul(out, 2)
        return out

def test_hccl_allreduce_8p():
    net = AllReduceNet()
    input_x = np.ones([3, 4]).astype(np.float32)
    expect_output = [[256, 256, 256, 256], [256, 256, 256, 256], [256, 256, 256, 256]]
    output = net(Tensor(input_x, mstype.float32))
    assert np.allclose(output.asnumpy(), expect_output)
