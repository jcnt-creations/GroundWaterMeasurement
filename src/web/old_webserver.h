#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <mysql_connection.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
// #include <thread>
#include <unistd.h>
#include <vector>

class WebServer {
  private:
	sql::Connection *con;
	std::vector<char> vBuffer;
	int port;
	int serverSocket;

  public:
	WebServer(sql::Connection *con, int port) : con(con), port(port) {};
	~WebServer() {
		try {
			delete con;
			vBuffer.clear();
			// delete vBuffer;
		} catch (const std::exception &e) {
			std::cerr << e.what() << '\n';
		}
	};
	// void HandleClient(sys::socket socket);
	// void HandleClient(int *clientSocket);
	void Test();
	void operator()() {

		try {
			serverSocket = socket(AF_INET, SOCK_STREAM, 0);
			sockaddr_in serverAddr;
			serverAddr.sin_family = AF_INET;		 // set IP family - IPv4
			serverAddr.sin_port = htons(port);		 // set port number
			serverAddr.sin_addr.s_addr = INADDR_ANY; // listen to all IPs

			bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

			std::cout << "Webserver: Webserver started on port " << port << std::endl;
			/*listen(serverSocket, 5);

			int clientSocket = accept(serverSocket, NULL, NULL);

			HandleClient(clientSocket);*/

		} catch (const std::exception &e) {
			std::cerr << e.what() << '\n';
		}
	}
};
