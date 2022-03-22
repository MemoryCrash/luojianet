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

#ifndef LUOJIANET_MS_CCSRC_DISTRIBUTED_RPC_TCP_SOCKET_OPERATION_H_
#define LUOJIANET_MS_CCSRC_DISTRIBUTED_RPC_TCP_SOCKET_OPERATION_H_

#include <netinet/in.h>
#include <string>

namespace luojianet_ms {
namespace distributed {
namespace rpc {
class Connection;

union SocketAddress {
  struct sockaddr sa;
  struct sockaddr_in saIn;
  struct sockaddr_in6 saIn6;
  struct sockaddr_storage saStorage;
};

class SocketOperation {
 public:
  SocketOperation() = default;
  virtual ~SocketOperation() {}

  static std::string GetIP(const std::string &url);
  static uint16_t GetPort(int sock_fd);

  // Get the address(ip:port) of the other end of the connection.
  static std::string GetPeer(int sock_fd);

  // Get socket address of the url.
  static bool GetSockAddr(const std::string &url, SocketAddress *addr);

  // Create a server socket.
  static int CreateServerSocket(sa_family_t family);

  // Set socket options.
  static int SetSocketOptions(int sock_fd);
  static int SetSocketKeepAlive(int fd, int keepalive, int keepidle, int keepinterval, int keepcount);

  // Connect to the Socket sock_fd.
  static int Connect(int sock_fd, const struct sockaddr *sa, socklen_t saLen, uint16_t *boundPort);

  // Close the given connection.
  virtual void Close(Connection *connection) = 0;

  // Start and listen on the socket represented by the given url.
  static int Listen(const std::string &url);

  // Accept connection on the server socket.
  static int Accept(int sock_fd);

  // Call recv with flag MSG_PEEK which means do not delete data in buffer after reading.
  virtual int ReceivePeek(Connection *connection, char *recvBuf, uint32_t recvLen) = 0;

  // Try to receive messages up to totalRecvLen (for message header).
  virtual int Receive(Connection *connection, char *recvBuf, uint32_t totalRecvLen, uint32_t *recvLen) = 0;

  // Receive message (for message body).
  virtual int ReceiveMessage(Connection *connection, struct msghdr *recvMsg, uint32_t recvLen) = 0;

  virtual int SendMessage(Connection *connection, struct msghdr *sendMsg, uint32_t *sendLen) = 0;

  // Handle connect and connected events.
  virtual void NewConnEventHandler(int fd, uint32_t events, void *context) = 0;
  virtual void ConnEstablishedEventHandler(int fd, uint32_t events, void *context) = 0;
};
}  // namespace rpc
}  // namespace distributed
}  // namespace luojianet_ms

#endif