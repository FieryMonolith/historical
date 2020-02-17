#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>

#include "tcp_shared.h"
#include "tcp_error.h"
#include "tcp_server.h"

using namespace std;

Server::Server(int state) {
  d_ok = true;
  d_state = state;
  MAXPENDING = 5;

  if(state == 1) {
    d_echoServerPort = SERVER1_PORT;
  } else {
    d_echoServerPort = SERVER2_PORT;
  }
  if((d_serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    DieWithError("socket() failed");
    d_ok = false;
    return;
  }

  memset(&d_echoServerAddress, 0, sizeof(d_echoServerAddress));
  d_echoServerAddress.sin_family = AF_INET;
  d_echoServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  d_echoServerAddress.sin_port = htons(d_echoServerPort);

  if(bind(d_serverSocket, (struct sockaddr *)&d_echoServerAddress,
	  sizeof(d_echoServerAddress)) < 0) {
    DieWithError("bind() failed");
    d_ok = false;
    return;
  }

  if(listen(d_serverSocket, MAXPENDING) < 0) {
    DieWithError("listen() failed");
    d_ok = false;
    return;
  }

  return;
}

Server::~Server(void) {
  return;
}

void Server::Listen(void) {
  while(1) {
    d_clientLength = sizeof(d_echoClientAddress);
    if((d_clientSocket = accept(d_serverSocket, \
				(struct sockaddr *)&d_echoClientAddress,
				&d_clientLength)) < 0) {
      DieWithError("accept() failed");
      d_ok = false;
      break;
    }
    cout << "Handling client " << inet_ntoa(d_echoClientAddress.sin_addr);
    cout << endl;

    if(HandleTCPClient()) {
      break;
    }
  }

  return;
}

bool Server::Ok(void) {
  return d_ok;
}

int Server::HandleTCPClient(void) {
  char output[BUFFER_SIZE];
  char input[BUFFER_SIZE];
  char line[256];
  int size;
  char *temp;

  if((size = recv(d_clientSocket, input, BUFFER_SIZE - 1, 0)) <= 0) {
    DieWithError("Server: recv() failed 1");
    return 1;
  }
  input[size] = '\0';

  if(strstr(input, "REQUEST_FILE=") == NULL) {
    cout << "REQUIRES a filename.  Ending request!" << endl;
    send(d_clientSocket, DONE_STRING, strlen(DONE_STRING), 0);
    close(d_clientSocket);
    d_clientSocket = -1;

    return 0;
  }
  strcpy(d_filename, &input[strlen("REQUEST_FILE=")]);
  temp = strstr(d_filename, "=");
  if(temp != NULL) {
    temp[0] = '\0';
  } else {
    temp = strstr(d_filename, "\n");
    if(temp != NULL) {
      temp[0] = '\0';
    }
  }
  cout << "Received: " << d_filename << endl;

  ifstream in(d_filename);
  if(!in) {
    cout << "Could not open " << d_filename << endl;
  } else {
    while(in) {
      in.getline(line, 255);
      cout << "Line: " << line << endl;
      strcpy(output, line);
      strcat(output, "\n");

      if(send(d_clientSocket, output, strlen(output), 0) != strlen(output)) {
	DieWithError("send() sent a different number of bytes than expected");
	return 1;
      }
    }

    in.close();
  }

  if(d_state == 1) {
    Move();
  } else {
    if(send(d_clientSocket, DONE_STRING, strlen(DONE_STRING), 0) != 
       strlen(DONE_STRING)) {
      DieWithError("send() sent a different number of bytes than expected");
      return 1;
    }
  }

  close(d_clientSocket);
  d_clientSocket = -1;

  return 0;
}

int Server::Move(void) {
  int sSocket;
  struct sockaddr_in serverAddress;
  unsigned short serverPort;
  char *serverIP;
  char output[BUFFER_SIZE];
  char input[BUFFER_SIZE];
  int size;
  int loc;
  int returnSize;

  serverIP = SERVER2_IP;
  serverPort = SERVER2_PORT;

  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family      = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr(serverIP);
  serverAddress.sin_port        = htons(serverPort);

  loc = 0;
  BuildMovingMessage(output, &loc, &serverAddress);

  returnSize = send(d_clientSocket, output, loc, 0);
  if(returnSize != loc) {
    printf("Send failed size: %d\n", returnSize);
    DieWithError("Server1: send() failed");
    return 1;
  }

  if((size = recv(d_clientSocket, input, BUFFER_SIZE - 1, 0)) <= 0) {
    DieWithError("Server: recv() failed");
    return 1;
  }
  input[size] = '\0';
  cout << "Server: Received: " << input << endl;

  if(strcmp(input, "MOVE_OK") != 0) {
    // Should handle this error
    cout << "Not MOVE_OK, exiting" << endl;
    return 1;
  }

  return 0;
}

void Server::BuildMovingMessage(char *message, int *length,
				struct sockaddr_in *serverAddress) {
  unsigned long *x;
  unsigned short *y;

  strcpy(message, "7777777=");
  strcat(message, MOVING);
  strcat(message, "=");

  *length = strlen(message);
  x  = (unsigned long *)&message[*length];
  *x = serverAddress->sin_addr.s_addr;
  *length += sizeof(unsigned long);
  y = (unsigned short *)&message[*length];
  *y = serverAddress->sin_port;
  *length += sizeof(unsigned short);

  strcpy(&message[*length], DATA2);
  *length += strlen(DATA2);

  strcat(&message[*length], "7777777=\n");
  *length += 9;

  return;
}







