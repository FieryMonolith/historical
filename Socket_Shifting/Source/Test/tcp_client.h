#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <arpa/inet.h>
#include "tcp_shared.h"

class Client {
public:
  Client(int state);
  ~Client(void);

  void HandleTCPServer(void);
  bool Ok(void);

private:
  bool d_ok;

  int d_socket;
  struct sockaddr_in d_echoServerAddress;
  unsigned short d_echoServerPort;
  char *d_serverIP;
  char *d_echoString;
  char d_echoBuffer[BUFFER_SIZE];
  unsigned int d_echoStringLength;
  int d_bytesReceived, d_totalBytesReceived;
};

#endif
