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

#ifndef LUOJIANET_MS_CCSRC_FL_SERVER_KERNEL_PUSH_LIST_SIGN_KERNEL_H
#define LUOJIANET_MS_CCSRC_FL_SERVER_KERNEL_PUSH_LIST_SIGN_KERNEL_H

#include <vector>
#include <string>
#include <memory>
#include "fl/server/common.h"
#include "fl/server/kernel/round/round_kernel.h"
#include "fl/server/kernel/round/round_kernel_factory.h"
#include "fl/armour/cipher/cipher_init.h"
#include "fl/server/executor.h"

namespace luojianet_ms {
namespace fl {
namespace server {
namespace kernel {
// results of signature verification
enum sigVerifyResult { FAILED, TIMEOUT, PASSED };

class PushListSignKernel : public RoundKernel {
 public:
  PushListSignKernel() = default;
  ~PushListSignKernel() override = default;
  void InitKernel(size_t required_cnt) override;
  bool Launch(const std::vector<AddressPtr> &inputs, const std::vector<AddressPtr> &,
              const std::vector<AddressPtr> &outputs) override;
  bool LaunchForPushListSign(const schema::SendClientListSign *client_list_sign_req, const size_t &iter_num,
                             const std::shared_ptr<server::FBBuilder> &fbb, const std::vector<AddressPtr> &outputs);
  bool Reset() override;
  void BuildPushListSignKernelRsp(const std::shared_ptr<server::FBBuilder> &fbb, const schema::ResponseCode retcode,
                                  const string &reason, const string &next_req_time, const size_t iteration);

 private:
  armour::CipherInit *cipher_init_;
  Executor *executor_;
  size_t iteration_time_window_;
  sigVerifyResult VerifySignature(const schema::SendClientListSign *client_list_sign_req);
  bool PushListSign(const size_t cur_iterator, const std::string &next_req_time,
                    const schema::SendClientListSign *client_list_sign_req,
                    const std::shared_ptr<fl::server::FBBuilder> &fbb,
                    const std::vector<std::string> &update_model_clients);
};
}  // namespace kernel
}  // namespace server
}  // namespace fl
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_FL_SERVER_KERNEL_PUSH_LIST_SIGN_KERNEL_H
