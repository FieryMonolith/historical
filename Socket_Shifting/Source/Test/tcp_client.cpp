#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "tcp_error.h"
#include "tcp_client.h"

using namespace std;

Client::Client(int state) {
  d_ok = true;

  if(state == 1) {
    d_serverIP = SERVER1_IP;
    d_echoServerPort = SERVER1_PORT;
  } else {
    d_serverIP = SERVER2_IP;
    d_echoServerPort = SERVER2_PORT;
  }

  d_echoString = "REQUEST_FILE=server1.txt=\n";
  d_echoStringLength = strlen(d_echoString);
  d_bytesReceived = 0;
  d_totalBytesReceived = 0;

  if((d_socket  = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    DieWithError("socket() failed");
    d_ok = false;
    return;
  }

  memset(&d_echoServerAddress, 0, sizeof(d_echoServerAddress));
  d_echoServerAddress.sin_family      = AF_INET;
  d_echoServerAddress.sin_addr.s_addr = inet_addr(d_serverIP);
  d_echoServerAddress.sin_port        = htons(d_echoServerPort);

  if(connect(d_socket, (struct sockaddr *)&d_echoServerAddress, 
	     sizeof(d_echoServerAddress)) < 0) {
    DieWithError("connect() failed");
    d_ok = false;
    return;
  }

  return;
}

Client::~Client(void) {
  close(d_socket);

  return;
}

void Client::HandleTCPServer(void) {
  if(send(d_socket, d_echoString, d_echoStringLength, 0) != 
     d_echoStringLength) {
    DieWithError("send() sent a different number of bytes than expected");
    return;
  }

  int done = 0;
  while(!done) {
    char *token;
    d_bytesReceived = recv(d_socket, d_echoBuffer, BUFFER_SIZE - 1 , 0);
    if(d_bytesReceived <= 0) {
      printf("recv size: %d, buffer size: %d\n", d_bytesReceived, BUFFER_SIZE);
      DieWithError("recv() failed or connection closed prematurely");
      return;
    }
    d_echoBuffer[d_bytesReceived] = '\0';
    token = strtok(d_echoBuffer, "\n");
    while(token != NULL) {
      if(strcmp(token, DONE_COMPARE_STRING) == 0) {
	cout << "Done" << endl;
	done = 1;
      } else {
	cout << "Received(tcp): ";
	cout << token;
	cout << endl;
      }
      token = strtok(NULL, "\n");
    }
  }

  return;
}

bool Client::Ok(void) {
  return d_ok;
}
