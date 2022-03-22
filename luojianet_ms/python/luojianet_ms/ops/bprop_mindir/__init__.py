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

"""bprop mindir."""
from .generate_mindir import serializable_bprop_ops
import os
import sys

__all__ = ['serializable_bprop_ops']

working_dir = os.getcwd()
os.chdir(os.path.dirname(os.path.abspath(__file__)))
os.system('python generate_mindir.py')
os.chdir(working_dir)