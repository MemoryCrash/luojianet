/**
 * Copyright 2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LUOJIANET_MS_LITE_JAVA_SRC_COMMON_MS_LOG_H
#define LUOJIANET_MS_LITE_JAVA_SRC_COMMON_MS_LOG_H

#define TAG "MS_LITE"

#ifdef PLATFORM_ARM
#include <android/log.h>
#include <unistd.h>

#define MS_LOGD(fmt, args...) \
  { __android_log_print(ANDROID_LOG_DEBUG, TAG, "|%d|%s[%d]|: " fmt, getpid(), __func__, __LINE__, ##args); }

#define MS_LOGE(fmt, args...) \
  { __android_log_print(ANDROID_LOG_ERROR, TAG, "|%d|%s[%d]|: " fmt, getpid(), __func__, __LINE__, ##args); }

#define MS_LOGI(fmt, args...) \
  { __android_log_print(ANDROID_LOG_INFO, TAG, "|%d|%s[%d]|: " fmt, getpid(), __func__, __LINE__, ##args); }
#else
#define MS_LOGD(fmt, args...) \
  { printf("[DEBUG] %s|%s|%s[%d]|: " #fmt "\r\n", TAG, __FILE__, __func__, __LINE__, ##args); }

#define MS_LOGE(fmt, args...) \
  { printf("[ERROR] %s|%s|%s[%d]|: " #fmt "\r\n", TAG, __FILE__, __func__, __LINE__, ##args); }

#define MS_LOGI(fmt, args...) \
  { printf("[INFO] %s|%s|%s[%d]|: " #fmt "\r\n", TAG, __FILE__, __func__, __LINE__, ##args); }
#endif

#endif  // LUOJIANET_MS_LITE_JAVA_SRC_COMMON_MS_LOG_H
