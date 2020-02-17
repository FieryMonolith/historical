#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <arpa/inet.h>

class Server {
public:
  Server(int state);
  ~Server(void);

  int HandleTCPClient(void);
  void Listen(void);
  bool Ok(void);
  int Move(void);

private:
  int d_state;

  int MAXPENDING;

  bool d_ok;
  int d_serverSocket;
  int d_clientSocket;
  struct sockaddr_in d_echoServerAddress;
  struct sockaddr_in d_echoClientAddress;
  unsigned short d_echoServerPort;
  unsigned int d_clientLength;
  char d_filename[256];


  void BuildMovingMessage(char *message, int *length,
			  struct sockaddr_in *serverAddress);
};

#endif

