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

#ifndef LUOJIANET_MS_CCSRC_FL_SERVER_COLLECTIVE_OPS_IMPL_H_
#define LUOJIANET_MS_CCSRC_FL_SERVER_COLLECTIVE_OPS_IMPL_H_

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "proto/ps.pb.h"
#include "ps/ps_context.h"
#include "ps/core/server_node.h"
#include "fl/server/common.h"

namespace luojianet_ms {
namespace fl {
namespace server {
// The timeout for server collective communication in case of network jitter.
constexpr uint32_t kCollectiveCommTimeout = 30;

// The collective communication groups which are composed of multiple processes. Refer to MPI_Group.
struct CommunicationGroupInfo {
  // This group's rank size.
  uint32_t size;

  // This process's global rank id.
  uint32_t global_rank;

  // The group ranks consists of global ranks of the processes.
  std::vector<uint32_t> group_ranks;

  // The mapping of global ranks and group ranks.
  std::map<uint32_t, uint32_t> global_to_group_ranks;
  std::map<uint32_t, uint32_t> group_to_global_ranks;
};

// CollectiveOpsImpl is the collective communication API of the server.
// For now, it implements two AllReduce algorithms: RingAllReduce and BroadcastAllReduce. Elastic AllReduce is also
// supported for the elastic scaling feature of the server.
class CollectiveOpsImpl {
 public:
  static CollectiveOpsImpl &GetInstance() {
    static CollectiveOpsImpl instance;
    return instance;
  }

  void Initialize(const std::shared_ptr<ps::core::ServerNode> &server_node);

  template <typename T>
  bool AllReduce(const void *sendbuff, void *recvbuff, size_t count);

  template <typename T>
  bool AllGather(const void *sendbuff, void *recvbuff, size_t send_count,
                 const std::shared_ptr<ps::core::AbstractNode> &node);

  // Collective broadcast within the specified group. The parameter "root" is the group rank of the root process.
  // Normally 0.
  template <typename T>
  bool Broadcast(const void *sendbuff, void *recvbuff, size_t count, uint32_t root,
                 const std::shared_ptr<ps::core::AbstractNode> &node, const CommunicationGroupInfo &group_info);

  // Reinitialize the ring for collective communication after scaling operations are done.
  bool ReInitForScaling();

 private:
  CollectiveOpsImpl()
      : server_node_(nullptr),
        rank_id_(0),
        server_num_(0),
        node_(nullptr),
        node_role_(ps::core::NodeRole::WORKER),
        rank_size_(0) {}
  ~CollectiveOpsImpl() = default;
  CollectiveOpsImpl(const CollectiveOpsImpl &) = delete;
  CollectiveOpsImpl &operator=(const CollectiveOpsImpl &) = delete;

  // Implementation of RingAllReduce.
  template <typename T>
  bool RingAllReduce(const void *sendbuff, void *recvbuff, size_t count);

  // Implementation of BroadcastAllReduce.
  template <typename T>
  bool ReduceBroadcastAllReduce(const void *sendbuff, void *recvbuff, size_t count);

  // Implementation of RingAllGather.
  template <typename T>
  bool RingAllGather(const void *sendbuff, void *recvbuff, size_t send_count);

  // Implementation of Broadcast. The parameter "root" is the group rank of the root process. Normally 0.
  template <typename T>
  bool Broadcast(const void *sendbuff, void *recvbuff, size_t count, uint32_t root,
                 const CommunicationGroupInfo &group_info);

  std::shared_ptr<ps::core::ServerNode> server_node_;
  uint32_t rank_id_;
  uint32_t server_num_;

  // The mutex to ensure that collective communication is threadsafe.
  std::mutex mtx_;

  // The abstract node could be worker or server. Only nodes which have the same role could use collective
  // communication.
  std::shared_ptr<ps::core::AbstractNode> node_;
  ps::core::NodeRole node_role_;
  uint32_t rank_size_;
};
}  // namespace server
}  // namespace fl
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_FL_SERVER_COLLECTIVE_OPS_IMPL_H_
