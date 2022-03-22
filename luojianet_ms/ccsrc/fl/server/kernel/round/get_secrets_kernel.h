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

#ifndef LUOJIANET_MS_CCSRC_FL_SERVER_KERNEL_GET_SECRETS_KERNEL_H
#define LUOJIANET_MS_CCSRC_FL_SERVER_KERNEL_GET_SECRETS_KERNEL_H

#include <vector>
#include <memory>
#include "fl/server/common.h"
#include "fl/server/kernel/round/round_kernel.h"
#include "fl/server/kernel/round/round_kernel_factory.h"
#include "fl/armour/cipher/cipher_shares.h"
#include "fl/server/executor.h"

namespace luojianet_ms {
namespace fl {
namespace server {
namespace kernel {
// results of signature verification
enum sigVerifyResult { FAILED, TIMEOUT, PASSED };

class GetSecretsKernel : public RoundKernel {
 public:
  GetSecretsKernel() = default;
  ~GetSecretsKernel() override = default;
  void InitKernel(size_t required_cnt) override;
  bool Launch(const std::vector<AddressPtr> &inputs, const std::vector<AddressPtr> &,
              const std::vector<AddressPtr> &outputs) override;
  bool Reset() override;

 private:
  Executor *executor_;
  size_t iteration_time_window_;
  armour::CipherShares *cipher_share_;
  sigVerifyResult VerifySignature(const schema::GetShareSecrets *get_secrets_req);
  bool CountForGetSecrets(const std::shared_ptr<FBBuilder> &fbb, const schema::GetShareSecrets *get_secrets_req,
                          const size_t iter_num);
};
}  // namespace kernel
}  // namespace server
}  // namespace fl
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_FL_SERVER_KERNEL_GET_SECRETS_KERNEL_H