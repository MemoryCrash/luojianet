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

usage()
{
  echo "Usage:"
  echo "bash build.sh [-e npu] "
  echo ""
  echo "Options:"
  echo "    If set to -e npu, we will download the library of CPU+NPU, otherwise it will download the library of CPU+GPU by default."
}

BASEPATH=$(
  cd "$(dirname $0)" || exit
  pwd
)
get_version() {
  VERSION_MAJOR=$(grep "const int ms_version_major =" ${BASEPATH}/../../include/version.h | tr -dc "[0-9]")
  VERSION_MINOR=$(grep "const int ms_version_minor =" ${BASEPATH}/../../include/version.h | tr -dc "[0-9]")
  VERSION_REVISION=$(grep "const int ms_version_revision =" ${BASEPATH}/../../include/version.h | tr -dc "[0-9]")
  VERSION_STR=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}
}

# check and set options
checkopts()
{
  # Init default values of build options
  DEVICE="GPU"
  SUPPORT_NPU="off"
  LUOJIANET_MS_FILE_NAME="luojianet_ms-lite-${VERSION_STR}-android-aarch64"
  LUOJIANET_MS_FILE="${LUOJIANET_MS_FILE_NAME}.tar.gz"
  LUOJIANET_MS_LITE_DOWNLOAD_URL="https://ms-release.obs.cn-north-4.myhuaweicloud.com/${VERSION_STR}/LuoJiaNet/lite/release/android/gpu/${LUOJIANET_MS_FILE}"
  # Process the options
  while getopts 'e:h' opt
  do
    OPTARG=$(echo ${OPTARG} | tr '[A-Z]' '[a-z]')
    case "${opt}" in
      e)
        DEVICE=$OPTARG
        if [[ "X${DEVICE}" == "Xgpu" ]]; then
          continue
        elif [[ "X${DEVICE}" == "Xnpu" ]]; then
          LUOJIANET_MS_FILE_NAME="luojianet_ms-lite-${VERSION_STR}-android-aarch64"
          LUOJIANET_MS_LITE_DOWNLOAD_URL="https://ms-release.obs.cn-north-4.myhuaweicloud.com/${VERSION_STR}/LuoJiaNet/lite/release/android/npu/${LUOJIANET_MS_FILE}"
          SUPPORT_NPU="on"
        else
          echo "Unknown DEVICE option ${OPTARG}!"
          usage
          exit 1
        fi
        ;;
      h)
        usage
        exit 0
        ;;
      *)
        echo "Unknown option ${opt}!"
        usage
        exit 1
    esac
  done
}

get_version
checkopts "$@"

MODEL_DOWNLOAD_URL="https://download.luojianet_ms.cn/model_zoo/official/lite/quick_start/mobilenetv2.ms"

mkdir -p build
mkdir -p lib
mkdir -p model
if [ ! -e ${BASEPATH}/model/mobilenetv2.ms ]; then
  wget -c -O ${BASEPATH}/model/mobilenetv2.ms --no-check-certificate ${MODEL_DOWNLOAD_URL}
fi
if [ ! -e ${BASEPATH}/build/${LUOJIANET_MS_FILE} ]; then
  wget -c -O ${BASEPATH}/build/${LUOJIANET_MS_FILE} --no-check-certificate ${LUOJIANET_MS_LITE_DOWNLOAD_URL}
fi
tar xzvf ${BASEPATH}/build/${LUOJIANET_MS_FILE} -C ${BASEPATH}/build/
cp -r ${BASEPATH}/build/${LUOJIANET_MS_FILE_NAME}/runtime/lib/libluojianet-lite.a ${BASEPATH}/lib
cp -r ${BASEPATH}/build/${LUOJIANET_MS_FILE_NAME}/runtime/include ${BASEPATH}/
if [[ "X${DEVICE}" == "Xnpu" ]]; then
    cp -r ${BASEPATH}/build/${LUOJIANET_MS_FILE_NAME}/runtime/third_party/hiai_ddk/lib/*.so ${BASEPATH}/lib
fi
cd ${BASEPATH}/build || exit
cmake -DCMAKE_TOOLCHAIN_FILE="${ANDROID_NDK}/build/cmake/android.toolchain.cmake" -DANDROID_NATIVE_API_LEVEL="19" \
  -DANDROID_NDK="${ANDROID_NDK}" -DANDROID_ABI="arm64-v8a" -DANDROID_STL="c++_shared" ${BASEPATH} -DSUPPORT_NPU=${SUPPORT_NPU}

make && make install && make package
