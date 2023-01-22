#include "server.h"


void Server::run() {
  int client, server;
  struct sockaddr_in server_addres;
  client = socket(AF_INET, SOCK_STREAM, 0);
  if (client < 0) {
    std::cout << "Error socket not created";
  }
  std::cout << "socket has been created" << std::endl;

  server_addres.sin_addr.s_addr = htons(INADDR_ANY);
  server_addres.sin_family = AF_INET;
  server_addres.sin_port = htons(DEF_PORT);

  int ret = bind(client, reinterpret_cast<struct sockaddr*>(&server_addres), sizeof(server_addres)); 

  if (ret < 0) {
    std::cout << "Error bind faild connection" << std::endl;
    return;
  }
  socklen_t size = sizeof(server_addres);
  std::cout << "SERVER:" << "Listening client" << std::endl;
  listen(client, 1);

  server = accept(client, reinterpret_cast<sockaddr*>(&server_addres), &size);
  if (server < 0) {
    std::cout << "Error to connection";
  }
  char buffer[BUFFERSIZE];
  bool is_exit = false;
  MyRedis redis;
  while (server > 0)
  {
    strcpy(buffer, "server connected!\n");
    send(server, buffer, BUFFERSIZE, 0);
    std::cout << "client connected" << std::endl;
    std::cout << "Enter" << CLIENT_EXIT << "to disconect\n";

    std::cout << "Client:";
    recv(server, buffer, BUFFERSIZE, 0);
    std::string stmnt = buffer;
    redis.execute(stmnt);
    if (is_client_connected(buffer)) {
      is_exit = true;
    }
  
    while (!is_exit)
    {
      try { 
        std::cout << "Server:";
        std::cin.getline(buffer, BUFFERSIZE);
        send(server, buffer, BUFFERSIZE, 0);
        if (is_client_connected(buffer)) {
          break;
        }
        std::cout << "Client: ";
        recv(server, buffer, BUFFERSIZE, 0);  
        std::string statement = buffer;
        redis.execute(statement);
        if (is_client_connected(buffer)) {
          break;
        }
      }  catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
      std::cout << "\n Disconnected" << std::endl << "good bye";
      is_exit = false;
      exit(1);
    }
}

bool Server::is_client_connected(const char* msg) {
  for(int i = 0; i < strlen(msg); ++i) {
      if(msg[i] == CLIENT_EXIT) {
        return true;
      }
  }
  return false;
}