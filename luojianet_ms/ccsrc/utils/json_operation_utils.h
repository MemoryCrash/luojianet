/**
 * Copyright 2021, 2022 LuoJiaNET Research and Development Group, Wuhan University
 * Copyright 2021, 2022 Huawei Technologies Co., Ltd
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

#ifndef LUOJIANET_MS_JSON_OPERATION_UTILS_H
#define LUOJIANET_MS_JSON_OPERATION_UTILS_H
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "ir/dtype.h"

namespace luojianet_ms {

template <typename T>
T GetJsonValue(const nlohmann::json &json, const std::string &key) {
  auto obj_json = json.find(key);
  if (obj_json != json.end()) {
    try {
      T value = obj_json.value();
      return value;
    } catch (std::exception &e) {
      MS_LOG(ERROR) << "Get Json Value Error, error info: " << e.what();
      MS_LOG(EXCEPTION) << "Get Json Value Error, target type: " << typeid(T).name() << ", key: [" << key << "]"
                        << ", json dump: " << json.dump();
    }
  } else {
    MS_LOG(EXCEPTION) << "Get Json Value Error, can not find key [" << key << "], json dump: " << json.dump();
  }
}

bool ParseJson(const std::string &str, nlohmann::json *des_json);

}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_JSON_OPERATION_UTILS_H
