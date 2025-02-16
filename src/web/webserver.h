#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <mysql_connection.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

class webserver {
  private:
	sql::Connection *con;
	std::vector<char> vBuffer;
	int port;
	int serverSocket;

  public:
	webserver(sql::Connection *con, int port);
	~webserver();
	void HandleClient(socket socket);
	// void HandleClient(int clientSocket);
	void Test();
};

webserver::webserver(sql::Connection *con, int port) {
	this->con = con;
	this->port = port;

	try {
		serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		socketadd_in serverAddr;
		serverAddr.sin_family = AF_INET;		 // set IP family - IPv4
		serverAddr.sin_port = htons(port);		 // set port number
		serverAddr.sin_addr.s_addr = INADDR_ANY; // listen to all IPs

		bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
		/*listen(serverSocket, 5);

		int clientSocket = accept(serverSocket, NULL, NULL);

		HandleClient(clientSocket);*/

	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
	}
}

webserver::~webserver() {
	try {
		delete con;
		delete vBuffer;
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
	}

	delete con;
}
