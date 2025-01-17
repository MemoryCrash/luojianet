#!/bin/bash
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

set -e
BASEPATH=$(cd "$(dirname "$0")"; pwd)
PROJECT_PATH=${BASEPATH}/../../..
if [ $BUILD_PATH ];then
    echo "BUILD_PATH = $BUILD_PATH"
else
    BUILD_PATH=${PROJECT_PATH}/build
    echo "BUILD_PATH = $BUILD_PATH"
fi
cd ${BUILD_PATH}/luojianet_ms/tests/ut/cpp


export LD_LIBRARY_PATH=${BUILD_PATH}/luojianet_ms/googletest/googlemock/gtest:${PROJECT_PATH}/luojianet_ms/python/luojianet_ms:\
${PROJECT_PATH}/luojianet_ms/python/luojianet_ms/lib:${PROJECT_PATH}/graphengine/third_party/prebuild/x86_64:\
${PROJECT_PATH}/graphengine/third_party/prebuild/aarch64:${LD_LIBRARY_PATH}
export PYTHONPATH=${PROJECT_PATH}/tests/ut/cpp/python_input:$PYTHONPATH:${PROJECT_PATH}/luojianet_ms/python:\
${PROJECT_PATH}/tests/ut/python:${PROJECT_PATH}
export GLOG_v=2
export GC_COLLECT_IN_CELL=1
## set op info config path
export LUOJIANET_OP_INFO_PATH=${PROJECT_PATH}/config/op_info.config

## prepare data for dataset & mindrecord
cp -fr $PROJECT_PATH/tests/ut/data ${PROJECT_PATH}/build/luojianet_ms/tests/ut/cpp/
## prepare album dataset, uses absolute path so has to be generated
python ${PROJECT_PATH}/build/luojianet_ms/tests/ut/cpp/data/dataset/testAlbum/gen_json.py

if [ $# -gt 0 ]; then 
  ./ut_tests --gtest_filter=$1
else
  ./ut_tests
fi
RET=$?
cd -

exit ${RET}
