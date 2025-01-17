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

#ifndef LUOJIANET_MS_LITE_SRC_RUNTIME_AGENT_ACL_ACL_ENV_GUARD_H_
#define LUOJIANET_MS_LITE_SRC_RUNTIME_AGENT_ACL_ACL_ENV_GUARD_H_

#include <memory>
#include <mutex>
#include "acl/acl_base.h"

namespace luojianet_ms::kernel {
namespace acl {
class AclEnvGuard {
 public:
  explicit AclEnvGuard(std::string_view cfg_file);
  ~AclEnvGuard();
  aclError GetErrno() const { return errno_; }
  static std::shared_ptr<AclEnvGuard> GetAclEnv(std::string_view cfg_file);

 private:
  static std::shared_ptr<AclEnvGuard> global_acl_env_;
  static std::mutex global_acl_env_mutex_;

  aclError errno_;
};
}  // namespace acl
}  // namespace luojianet_ms::kernel

#endif  // LITE_ACL_ENV_GUARD_H
