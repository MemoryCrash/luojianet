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

import math
import numpy as np

import luojianet_ms as ms
import luojianet_ms.nn as nn
from luojianet_ms import Tensor
from luojianet_ms import context
from luojianet_ms.common.api import _cell_graph_executor
from luojianet_ms.ops import composite as C
from luojianet_ms.ops import operations as P
from tests.ut.python.ops.test_math_ops import VirtualLoss


grad_all = C.GradOperation(get_all=True)


class NetWithLoss(nn.Module):
    def __init__(self, network):
        super(NetWithLoss, self).__init__()
        self.loss = VirtualLoss()
        self.network = network

    def call(self, x, y, b):
        predict = self.network(x, y, b)
        return self.loss(predict)


class GradWrap(nn.Module):
    def __init__(self, network):
        super(GradWrap, self).__init__()
        self.network = network

    def call(self, x, y, b):
        return grad_all(self.network)(x, y, b)


def loop_config(size):
    config_list = []
    num = 1
    split_list = [num]
    for _ in range(int(math.log2(size))):
        num = num * 2
        split_list.append(num)

    for a in split_list:
        for b in split_list:
            if a * b > size:
                continue
            c = int(size / (a * b))
            config_list.append(((a, b), (b, c)))

    return config_list


# model_parallel test
def test_two_matmul():
    class Net(nn.Module):
        def __init__(self, strategy1, strategy2):
            super().__init__()
            self.matmul1 = P.MatMul().shard(strategy1)
            self.matmul2 = P.MatMul().shard(strategy2)

        def call(self, x, y, b):
            out = self.matmul1(x, y)
            out = self.matmul2(out, b)
            return out

    size = 4
    context.set_auto_parallel_context(device_num=size, global_rank=0)
    x = Tensor(np.ones([128, 32]), dtype=ms.float32)
    y = Tensor(np.ones([32, 64]), dtype=ms.float32)
    b = Tensor(np.ones([64, 64]), dtype=ms.float32)

    config_list = loop_config(size)

    count = 0
    for strategy1 in config_list:
        for strategy2 in config_list:
            print("=======current config {}=========".format(count))
            print(strategy1, strategy2)
            net = GradWrap(NetWithLoss(Net(strategy1, strategy2)))
            context.set_auto_parallel_context(parallel_mode="semi_auto_parallel")
            net.set_auto_parallel()
            net.set_train()
            _cell_graph_executor.compile(net, x, y, b)
            count = count + 1
