#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <linux/socket.h>
#include <unistd.h>

#include "tcp_client.h"
#include "tcp_server.h"

int main(int argc, char **argv) {
  bool isServer = false;
  int state = 0;
  Server *server;
  Client *client;

  int i;
  for(i=0; i<argc; i++) {
    if(strcmp(argv[i], "-state1") == 0) {
      isServer = true;
      state = 1;
      server = new Server(state);
    } else if(strcmp(argv[i], "-state2") == 0) {
      isServer = true;
      state = 2;
      server = new Server(state);
    } else if(strcmp(argv[i], "2") == 0) {
      state = 1;
    }
  }

  if(!isServer) {
    if(state) {
      client = new Client(2);
    } else {
      client = new Client(1);
    }
    client->HandleTCPServer();
  } else {
    server->Listen();
  }

  return 0;
}

