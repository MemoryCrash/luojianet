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
""" test syntax for logic expression """

import luojianet_ms.nn as nn
from luojianet_ms import context

context.set_context(mode=context.GRAPH_MODE)


class IdentityIs(nn.Module):
    def __init__(self, x, y):
        super(IdentityIs, self).__init__()
        self.x = x
        self.y = y

    def call(self):
        in_v = self.x is self.y
        return in_v


def test_ms_syntax_operator_int_is_int():
    net = IdentityIs(1, 2)
    ret = net()
    print(ret)


def test_ms_syntax_operator_int_is_none():
    net = IdentityIs(1, None)
    ret = net()
    print(ret)


def test_ms_syntax_operator_int_is_true():
    net = IdentityIs(1, True)
    ret = net()
    print(ret)


def test_ms_syntax_operator_bool_is_none():
    net = IdentityIs(True, None)
    ret = net()
    print(ret)


def test_ms_syntax_operator_bool_is_false():
    net = IdentityIs(True, False)
    ret = net()
    print(ret)
