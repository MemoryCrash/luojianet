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
#include "actor/mailbox.h"

namespace luojianet_ms {
int BlockingMailBox::EnqueueMessage(std::unique_ptr<luojianet_ms::MessageBase> msg) {
  {
    std::unique_lock<std::mutex> ulk(lock);
    (void)enqueMailBox->emplace_back(std::move(msg));
  }

  cond.notify_all();
  return 0;
}

std::list<std::unique_ptr<MessageBase>> *BlockingMailBox::GetMsgs() {
  std::list<std::unique_ptr<MessageBase>> *ret;
  {
    std::unique_lock<std::mutex> ulk(lock);
    while (enqueMailBox->empty()) {
      cond.wait(ulk, [this] { return !this->enqueMailBox->empty(); });
    }
    SwapMailBox(&enqueMailBox, &dequeMailBox);
    ret = dequeMailBox;
  }
  return ret;
}

int NonblockingMailBox::EnqueueMessage(std::unique_ptr<luojianet_ms::MessageBase> msg) {
  bool empty = false;
  bool released = false;
  {
    std::unique_lock<std::mutex> ulk(lock);
    empty = enqueMailBox->empty();
    (void)enqueMailBox->emplace_back(std::move(msg));
    released = this->released_;
  }
  if (empty && released && notifyHook) {
    (*notifyHook.get())();
  }
  return 0;
}

std::list<std::unique_ptr<MessageBase>> *NonblockingMailBox::GetMsgs() {
  std::list<std::unique_ptr<MessageBase>> *ret;
  {
    std::unique_lock<std::mutex> ulk(lock);
    if (enqueMailBox->empty()) {
      released_ = true;
      return nullptr;
    }
    SwapMailBox(&enqueMailBox, &dequeMailBox);
    ret = dequeMailBox;
    released_ = false;
  }

  return ret;
}

int HQueMailBox::EnqueueMessage(std::unique_ptr<luojianet_ms::MessageBase> msg) {
  bool empty = mailbox.Empty();
  MessageBase *msgPtr = msg.release();
  while (!mailbox.Enqueue(msgPtr)) {
  }
  if (empty && notifyHook) {
    (*notifyHook.get())();
  }
  return 0;
}

std::unique_ptr<MessageBase> HQueMailBox::GetMsg() {
  std::unique_ptr<MessageBase> msg(mailbox.Dequeue());
  return msg;
}
}  // namespace luojianet_ms
