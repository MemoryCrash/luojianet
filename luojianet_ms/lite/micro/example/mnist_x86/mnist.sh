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

GEN=OFF
while getopts 'g' OPT
do
    case $OPT in
        g)
            GEN=ON;;
        ?)
            echo "Usage: add -g or left it empty"
    esac
done

BASEPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
LUOJIANET_MS_ROOT_DIR=${BASEPATH%%/luojianet_ms/lite/micro/example/mnist_x86}

echo "current dir is: ${BASEPATH}"

INPUT_BIN=${BASEPATH}/mnist_input.bin
MNIST_NAME=mnist
MNIST_FILE=${MNIST_NAME}.ms
BENCHMARK_PATH=${BASEPATH}

get_version() {
    local VERSION_HEADER=${LUOJIANET_MS_ROOT_DIR}/luojianet_ms/lite/include/version.h
    local VERSION_MAJOR=$(grep "const int ms_version_major =" ${VERSION_HEADER} | tr -dc "[0-9]")
    local VERSION_MINOR=$(grep "const int ms_version_minor =" ${VERSION_HEADER} | tr -dc "[0-9]")
    local VERSION_REVISION=$(grep "const int ms_version_revision =" ${VERSION_HEADER} | tr -dc "[0-9]")
    VERSION_STR=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}
}

download_inference() {
    LUOJIANET_MS_FILE_NAME="luojianet_ms-lite-${VERSION_STR}-linux-x64"
    local LUOJIANET_MS_FILE="${LUOJIANET_MS_FILE_NAME}.tar.gz"
    local LUOJIANET_MS_LITE_DOWNLOAD_URL="https://ms-release.obs.cn-north-4.myhuaweicloud.com/${VERSION_STR}/LuoJiaNet/lite/release/linux/${LUOJIANET_MS_FILE}"

    if [ ! -e ${BASEPATH}/build/${LUOJIANET_MS_FILE} ]; then
      wget -c -O ${BASEPATH}/build/${LUOJIANET_MS_FILE} --no-check-certificate ${LUOJIANET_MS_LITE_DOWNLOAD_URL}
    fi

    tar xzvf ${BASEPATH}/build/${LUOJIANET_MS_FILE} -C ${BASEPATH}/build/ || exit 1
    rm ${BASEPATH}/build/${LUOJIANET_MS_FILE} || exit 1
    PKG_PATH=${BASEPATH}/build/${LUOJIANET_MS_FILE_NAME}
}

download_mnist() {
    local MNIST_DOWNLOAD_URL=https://download.luojianet_ms.cn/model_zoo/official/lite/mnist_lite/${MNIST_FILE}

    if [ ! -e ${BASEPATH}/build/${MNIST_FILE} ]; then
      wget -c -O ${BASEPATH}/build/${MNIST_FILE} --no-check-certificate ${MNIST_DOWNLOAD_URL}
    fi
}

gen_mnist() {
    local CODEGEN_PATH=${BASEPATH}/build/${LUOJIANET_MS_FILE_NAME}/tools/codegen
    ${CODEGEN_PATH}/codegen --codePath=${BASEPATH}/build --modelPath=${BASEPATH}/build/${MNIST_FILE}
}

mkdir -p ${BASEPATH}/build

get_version
download_inference

if [[ "${GEN}" == "ON" ]]; then
    echo "downloading mnist.ms!"
    download_mnist
    echo "generating mnist"
    gen_mnist
    BENCHMARK_PATH=${BASEPATH}/build/${MNIST_NAME}
fi

# 1. build benchmark
rm -rf ${BASEPATH}/build/benchmark
mkdir -p ${BASEPATH}/build/benchmark && cd ${BASEPATH}/build/benchmark || exit 1
cmake -DPKG_PATH=${PKG_PATH} ${BENCHMARK_PATH}
make

# 2. run benchmark
echo "net file: ${BENCHMARK_PATH}/src/mnist.bin"
./benchmark ${INPUT_BIN} ${BENCHMARK_PATH}/src/net.bin
