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
"""
test luojianet_ms grammar constraints
1. function must have return statement
2. raise statement can not be used
"""
# pylint: disable=R1705, R1710, W0223
import numpy as np
import pytest

import luojianet_ms.nn as nn
from luojianet_ms import Tensor
from luojianet_ms import context
from luojianet_ms import dtype as mstype

context.set_context(mode=context.GRAPH_MODE)

def test_missing_return():
    class NetMissReturn(nn.Module):
        def __init__(self):
            super(NetMissReturn, self).__init__()

        def call(self, x, y, z):
            if x == 1:
                return 10
            elif x == 20:
                if y == 1:
                    return 3
                elif y == 2:
                    for i in range(z):
                        return i + z
                    i = 0
                    while i < z:
                        return i + z
                    def g(u):
                        return x + u
                    # here method 'call' misses a return statement
                    g(y)
                else:
                    return 7
            else:
                return 5

    net = NetMissReturn()
    x = Tensor(0, mstype.int32)
    y = Tensor(5, mstype.int32)
    z = Tensor(2, mstype.int32)
    with pytest.raises(TypeError) as er:
        net(x, y, z)
    assert "Function must has 'return' statement, but missing in bound method 'call'" in str(er.value)


def test_nest_function_missing_return():
    class NetNestFuncMissReturn(nn.Module):
        def __init__(self):
            super(NetNestFuncMissReturn, self).__init__()

        def call(self, x, y, z):
            if x == 1:
                return 10
            elif x == 20:
                if y == 1:
                    return 3
                elif y == 2:
                    for i in range(z):
                        return i + z
                    i = 0
                    while i < z:
                        return i + z
                    def g(u):
                        x += u
                        # nested function 'g' misses a return a statement
                    return g(y)
                else:
                    return 7
            else:
                return 5

    net = NetNestFuncMissReturn()
    x = Tensor(0, mstype.int32)
    y = Tensor(5, mstype.int32)
    z = Tensor(2, mstype.int32)
    with pytest.raises(TypeError) as er:
        net(x, y, z)
    assert "Function must has 'return' statement, but missing in function 'g'" in str(er.value)


def test_raise_in_method():
    class NetRaiseInMethod(nn.Module):
        def __init__(self):
            super(NetRaiseInMethod, self).__init__()

        def call(self, x, y, z):
            if x == 1:
                return 10
            elif x == 20:
                # add not support grammar 'raise' here
                raise ValueError('Illegal case')
            else:
                return y + z

    net = NetRaiseInMethod()
    x = Tensor(0, mstype.int32)
    y = Tensor(5, mstype.int32)
    z = Tensor(2, mstype.int32)
    with pytest.raises(RuntimeError) as er:
        net(x, y, z)
    assert "Unsupported statement 'Raise'." in str(er.value)


def test_raise_in_nested_function():
    class NetNestRaise(nn.Module):
        def __init__(self):
            super(NetNestRaise, self).__init__()

        def call(self, x, y, z):
            if x == 1:
                return 10
            elif x == 20:
                def nest_fn(u):
                    if u > 0:
                       # add not support grammar 'raise' here
                        raise ValueError('Illegal case')
                    return u + z + 1
                return nest_fn(y)
            else:
                return y + z

    net = NetNestRaise()
    x = Tensor(0, mstype.int32)
    y = Tensor(5, mstype.int32)
    z = Tensor(2, mstype.int32)
    with pytest.raises(RuntimeError) as er:
        net(x, y, z)
    assert "Unsupported statement 'Raise'." in str(er.value)


def test_nest_branch_with_return():
    class NetBranchWithReturn(nn.Module):
        def __init__(self):
            super(NetBranchWithReturn, self).__init__()

        def call(self, x, y, z):
            if x == 1:
                return 10
            else:
                return 5

    net = NetBranchWithReturn()
    x = Tensor(0, mstype.int32)
    y = Tensor(5, mstype.int32)
    z = Tensor(2, mstype.int32)
    net(x, y, z)


def test_any_with_no_return():
    class NetAnyNoReturn(nn.Module):
        def __init__(self):
            super(NetAnyNoReturn, self).__init__()

        def call(self, inp):
            result = inp.any()
            if result:
                return 6

    np_input = np.arange(2 * 3 * 4).reshape((2, 3, 4)).astype(np.bool_)
    tensor = Tensor(np_input)
    net = NetAnyNoReturn()
    with pytest.raises(TypeError) as er:
        net(tensor)
    assert "Function must has 'return' statement, but missing in bound method 'call'" in str(er.value)


def test_missing_construct():
    class NetMissConstruct(nn.Module):
        def __init__(self):
            super(NetMissConstruct, self).__init__()

        def construct1(self, inp):
            return 5

    np_input = np.arange(2 * 3 * 4).reshape((2, 3, 4)).astype(np.bool_)
    tensor = Tensor(np_input)
    net = NetMissConstruct()
    assert net(tensor) is None
