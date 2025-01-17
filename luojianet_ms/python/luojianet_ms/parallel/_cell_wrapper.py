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
"""Module of auto parallel"""

from luojianet_ms.nn.cell import Module
from luojianet_ms.ops.operations.comm_ops import AllGather
from luojianet_ms.communication import GlobalComm

_allgather_cell = None


class AllGatherCell(Module):
    """
    Allgather cell, used in model parallel scenario.
    To allgather the selected parameter slice from each device.
    """
    def __init__(self, group):
        super(AllGatherCell, self).__init__(auto_prefix=False)

        self.allgather = AllGather(group)

    def call(self, x):
        x = self.allgather(x)

        return x


class SaveOptShardCkptCell(Module):
    """
    Allgather cell, used in optimizer parallel scenario.
    Firstly gather the tensor to original layout in the specified device group.
    Then gather the whole parameter slices from all devices.

    Note:
        This could be optimized later with less communication consumption.
    """
    def __init__(self, group):
        super(SaveOptShardCkptCell, self).__init__(auto_prefix=False)
        self.allgather1 = AllGather(group)
        self.allgather2 = AllGather()

    def call(self, x):
        x = self.allgather1(x)
        x = self.allgather2(x)

        return x


def get_allgather_cell(group, need_merge_twice=False):
    """Get AllGatherCell object."""
    global _allgather_cell
    if need_merge_twice:
        _allgather_cell = SaveOptShardCkptCell(group)
    else:
        if group:
            _allgather_cell = AllGatherCell(group)
        else:
            _allgather_cell = AllGatherCell(GlobalComm.WORLD_COMM_GROUP)
    return _allgather_cell


def destroy_allgather_cell():
    """Destroy AllGatherCell object."""
    global _allgather_cell
    if _allgather_cell:
        _allgather_cell = None
