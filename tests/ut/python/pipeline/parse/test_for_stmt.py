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
""" test_for_stmt """
from dataclasses import dataclass
import numpy as np

from luojianet_ms import Tensor, Model, context
from luojianet_ms.ops import operations as P
from luojianet_ms.ops import composite as C
from luojianet_ms.ops import functional as F
from luojianet_ms.nn import Module
from luojianet_ms.nn import ReLU
from ...ut_filter import non_graph_engine


@dataclass
class Access:
    a: int
    b: int

    def max(self):
        if self.a > self.b:
            return self.a
        return self.b


class access2_net(Module):
    """ access2_net definition """

    def __init__(self, number, loop_count=1):
        super().__init__()
        self.number = number
        self.loop_count = loop_count
        self.relu = ReLU()

    def call(self, x):
        a = self.loop_count
        b = self.number
        z = Access(a, b)
        for _ in (a, z):
            x = self.relu(x)
        return x


def function_access_base(number):
    """ function_access_base """
    input_np = np.random.randn(2, 3, 4, 5).astype(np.float32)
    input_me = Tensor(input_np)
    if number == 2:
        net = access2_net(number)
    context.set_context(mode=context.GRAPH_MODE)
    model = Model(net)
    model.predict(input_me)


@non_graph_engine
def test_access_0040():
    """ test_access_0040 """
    function_access_base(2)


class OpSeqNet(Module):
    def __init__(self, loop_count=1):
        super().__init__()
        self.loop_count = loop_count
        self.op_seq = (P.Sqrt(), P.Reciprocal(), P.Square())

    def call(self, x):
        t = x
        for op in self.op_seq:
            t = op(t)
        return t


def test_op_seq_test():
    context.set_context(mode=context.GRAPH_MODE)
    net = OpSeqNet()
    input_np = np.random.randn(2, 3, 4, 5).astype(np.float32)
    input_me = Tensor(input_np)
    net(input_me)


_grad_fusion = C.MultitypeFuncGraph("grad_fushion")


@_grad_fusion.register("Tensor", "Function")
def tensor_grad_scale(x, op):
    return op(x)


class AllReduceTest(Module):
    def __init__(self, loop_count=1):
        super().__init__()
        self.op_list = ()
        self.fushion_flag = [0, 1, 1, 0, 1, 0]
        for i in self.fushion_flag:
            op = P.AllReduce().add_prim_attr('fusion', i)
            self.op_list = self.op_list + (op,)
        self.hyper_map = C.HyperMap()

    def call(self, x):
        ret = ()
        for _ in self.fushion_flag:
            ret = ret + (x,)
        fushion_res = self.hyper_map(F.partial(_grad_fusion), ret, self.op_list)
        return fushion_res


def test_allreduce_fushio_test():
    context.set_context(mode=context.GRAPH_MODE)
    net = AllReduceTest()
    input_np = np.random.randn(2, 3, 4, 5).astype(np.float32)
    input_me = Tensor(input_np)
    net(input_me)
