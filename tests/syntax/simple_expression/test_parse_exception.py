import numpy as np

import luojianet_ms.nn as nn
from luojianet_ms import context
from luojianet_ms.common.parameter import Parameter
from luojianet_ms.common.tensor import Tensor
import luojianet_ms.ops as ops
import luojianet_ms

context.set_context(mode=context.GRAPH_MODE)


class TestNoReturn(nn.Module):
    def __init__(self):
        super(TestNoReturn, self).__init__()
        self.m = 1

    def call(self, x, y):
        and_v = x * y
        and_v += 1
        # return and_v


def test_no_return():
    net = TestNoReturn()
    x = Tensor(np.ones([2, 2], np.float))
    y = Tensor(np.zeros([2, 2], np.float))
    ret = net(x, y)
    print(ret)


class TestSuper(nn.Module):
    def __init__(self):
        super().__init__()
        self.m = 1

    def call(self, x, y):
        super(TestSuper, 2, 3).aa()
        and_v = x * y
        return and_v


def test_super():
    net = TestSuper()
    x = Tensor(np.ones([2, 2], np.float))
    y = Tensor(np.zeros([2, 2], np.float))
    ret = net(x, y)
    print(ret)


class TestCompare(nn.Module):
    def __init__(self):
        super(TestCompare, self).__init__()
        self.m = 1

    def call(self, x, y):
        return x > y > 10


def test_compare():
    net = TestCompare()
    x = Tensor(np.ones([2, 2], np.float))
    y = Tensor(np.zeros([2, 2], np.float))
    ret = net(x, y)
    print(ret)


class TestUndefMemberChange(nn.Module):
    def __init__(self):
        super(TestUndefMemberChange, self).__init__()
        self.m = 1

    def call(self, x, y):
        self.t = x
        return x > y


def test_undef_member_changer():
    net = TestUndefMemberChange()
    x = Tensor(np.ones([2, 2], np.float))
    y = Tensor(np.zeros([2, 2], np.float))
    ret = net(x, y)
    print(ret)


class TestMemberChange(nn.Module):
    def __init__(self):
        super(TestMemberChange, self).__init__()
        self.t = Tensor(np.zeros([2, 2], np.float))

    def call(self, x, y):
        self.t = x
        return x > y


def test_member_changer():
    net = TestMemberChange()
    x = Tensor(np.ones([2, 2], np.float))
    y = Tensor(np.zeros([2, 2], np.float))
    ret = net(x, y)
    print(ret)


class TestUnsupportSTMT(nn.Module):
    def __init__(self):
        super(TestUnsupportSTMT, self).__init__()
        self.m = 1

    def call(self, x, y):
        try:
            val = x + y
        finally:
            val = x
        return val


def test_UnsupportSTMT():
    net = TestUnsupportSTMT()
    x = Tensor(np.ones([2, 2], np.float))
    y = Tensor(np.zeros([2, 2], np.float))
    ret = net(x, y)
    print(ret)


class TestUnsupportNum(nn.Module):
    def __init__(self):
        super(TestUnsupportNum, self).__init__()
        self.m = 1

    def call(self, x, y):
        a = x + 3.14j
        return a


def test_UnsupportNum():
    net = TestUnsupportNum()
    x = Tensor(np.ones([2, 2], np.float))
    y = Tensor(np.zeros([2, 2], np.float))
    ret = net(x, y)
    print(ret)


class TestAssignAdd(nn.Module):
    def __init__(self):
        super(TestAssignAdd, self).__init__()
        self.m = 1

    def call(self, x, y):
        x.id_ += y
        # x[1] += y
        return x


def test_AssignAdd():
    net = TestAssignAdd()
    ret = net([3, 1], 2)
    print(ret)


class TestParseListComp(nn.Module):
    def __init__(self):
        super(TestParseListComp, self).__init__()
        self.m = 1

    def call(self, x, y):
        ret = [m + y for l in x for m in l]
        return ret


def test_ParseListComp():
    net = TestParseListComp()

    ret = net([[1, 2], [3, 4]], 2)
    print(ret)


class TestAssign(nn.Module):
    def __init__(self):
        super(TestAssign, self).__init__()
        self.m = 1

    def call(self, x, y):
        x.id_ = y
        return x


def test_Assign():
    net = TestAssign()
    ret = net([3, 1], 2)
    print(ret)


class TestAssignList(nn.Module):
    def __init__(self):
        super(TestAssignList, self).__init__()
        self.m = 1

    def call(self, x, y):
        [m, n] = [x, y]
        return m, n


def test_AssignList():
    net = TestAssignList()
    ret = net([3, 1], 2)
    print(ret)


class TestParaDef(nn.Module):
    def __init__(self):
        super(TestParaDef, self).__init__()
        self.m = 1

    def call(self, x=1, y=1):
        ret = x + y
        return ret


def test_para_def():
    net = TestParaDef()
    ret = net(1, 2)
    print(ret)


class TestParameterNameNone(nn.Module):
    def __init__(self):
        super(TestParameterNameNone, self).__init__()
        self.matmul = ops.MatMul()
        # self.weight = Parameter(Tensor(np.ones((1, 2)), luojianet_ms.float32), name="w", requires_grad=True)
        self.weight = Parameter(Tensor(np.ones((1, 2)), luojianet_ms.float32), name=None, requires_grad=True)

    def call(self, x):
        out = self.matmul(self.weight, x)
        return out


def test_parameter_name_none():
    net = TestParameterNameNone()
    x = Tensor(np.ones((2, 1)), luojianet_ms.float32)
    print(net(x))


class TestBranchReturn(nn.Module):
    def __init__(self):
        super(TestBranchReturn, self).__init__()
        self.m = 1

    def call(self, x):
        if x > 0:
            return x + 1

        return x


def test_branch_return():
    net = TestBranchReturn()
    print(net(1))


class TestSliceNotInt(nn.Module):
    def __init__(self):
        super(TestSliceNotInt, self).__init__()
        self.m = 1

    def call(self, x):
        s = "ABCDEFGHIJKL"
        sl = slice(x, 4.5)
        return s[sl]


def test_slice_not_int():
    net = TestSliceNotInt()
    print(net(1))


class TestSliceNotIntDefInInit(nn.Module):
    def __init__(self):
        super(TestSliceNotIntDefInInit, self).__init__()
        self.sl = slice(1, 4.5)

    def call(self, x):
        s = "ABCDEFGHIJKL"
        return s[self.sl]


def test_slice_not_int_def_in_init():
    net = TestSliceNotIntDefInInit()
    print(net(1))


class MatMulCell(nn.Module):
    def __init__(self):
        super().__init__()
        self.m = 1

    def call(self, x):
        return x


class TestCellPipelineStage(nn.Module):
    def __init__(self, strategy1, strategy2, param=None):
        super().__init__()
        self.block = nn.CellList()
        cell = MatMulCell()
        cell.pipeline_stage = -1
        self.block.append(cell)
        cell = MatMulCell()
        cell.pipeline_stage = -1
        self.block.append(cell)

    def call(self, x):
        for i in range(2):
            x = self.block[i](x)
        return x


def test_cell_pipeline_state():
    strategy1 = Tensor((4, 1), luojianet_ms.int64)
    strategy2 = Tensor((2, 1), luojianet_ms.int64)
    net = TestCellPipelineStage(strategy1, strategy2)
    print(net(1))


class TestArgsKwArgs(nn.Module):
    def __init__(self):
        super(TestArgsKwArgs, self).__init__()
        self.m = 1

    def call(self, *args, **kwargs):
        x = 0
        for v in args:
            x += v

        # for k, v in kwargs.items():
        #     x += v
        return x


def test_args_kwargs():
    net = TestArgsKwArgs()
    print(net(1, 2, 3, 4, k1=5, k2=6))


class TestArgs(nn.Module):
    def __init__(self):
        super(TestArgs, self).__init__()
        self.m = 1

    def call(self, x, *args):
        for v in args:
            x += v

        return x


def test_args():
    net = TestArgs()
    print(net(1, 2, 3, 4))


class TestNoDef(nn.Module):
    def __init__(self):
        super().__init__()
        self.m = 1

    def call(self, x):
        x += self.y
        return x


def test_no_def():
    net = TestNoDef()
    print(net(1))
