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
"""test layer switch"""
import numpy as np

import luojianet_ms
from luojianet_ms import nn
from luojianet_ms import Tensor
from luojianet_ms import context
from luojianet_ms.ops import operations as P

class Layer1(nn.Module):
    def __init__(self):
        super(Layer1, self).__init__()
        self.net = nn.Conv2d(3, 1, 3, pad_mode='same')
        self.pad = nn.Pad(
            paddings=((0, 0), (0, 2), (0, 0), (0, 0)), mode="CONSTANT")

    def call(self, x):
        y = self.net(x)
        return self.pad(y)


class Layer2(nn.Module):
    def __init__(self):
        super(Layer2, self).__init__()
        self.net = nn.Conv2d(3, 1, 7, pad_mode='same')
        self.pad = nn.Pad(
            paddings=((0, 0), (0, 2), (0, 0), (0, 0)), mode="CONSTANT")

    def call(self, x):
        y = self.net(x)
        return self.pad(y)


class Layer3(nn.Module):
    def __init__(self):
        super(Layer3, self).__init__()
        self.net = nn.Conv2d(3, 3, 3, pad_mode='same')

    def call(self, x):
        return self.net(x)


class SwitchNet(nn.Module):
    def __init__(self):
        super(SwitchNet, self).__init__()
        self.layer1 = Layer1()
        self.layer2 = Layer2()
        self.layer3 = Layer3()
        self.layers = (self.layer1, self.layer2, self.layer3)
        self.fill = P.Fill()

    def call(self, x, index):
        y = self.layers[index](x)
        return y


class MySwitchNet(nn.Module):
    def __init__(self):
        super(MySwitchNet, self).__init__()
        self.layer1 = Layer1()
        self.layer2 = Layer2()
        self.layer3 = Layer3()
        self.layers = (self.layer1, self.layer2, self.layer3)
        self.fill = P.Fill()

    def call(self, x, index):
        y = self.layers[0](x)
        for i in range(len(self.layers)):
            if i == index:
                y = self.layers[i](x)
        return y


def test_layer_switch():
    context.set_context(mode=context.GRAPH_MODE)
    net = MySwitchNet()
    x = Tensor(np.ones((3, 3, 24, 24)), luojianet_ms.float32)
    index = Tensor(0, dtype=luojianet_ms.int32)
    net(x, index)

class MySwitchNetPynative(nn.Module):
    def __init__(self):
        super(MySwitchNetPynative, self).__init__()
        self.layer1 = Layer1()
        self.layer2 = Layer2()
        self.layer3 = Layer3()
        self.layers = (self.layer1, self.layer2, self.layer3)
        self.fill = P.Fill()

    def call(self, x, index):
        return self.layers[index](x)


def test_layer_switch_pynative():
    context.set_context(mode=context.PYNATIVE_MODE)
    net = MySwitchNetPynative()
    x = Tensor(np.ones((3, 3, 24, 24)), luojianet_ms.float32)
    index = Tensor(2, dtype=luojianet_ms.int32)
    net(x, index)
    context.set_context(mode=context.GRAPH_MODE)
