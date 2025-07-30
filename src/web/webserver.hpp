#include <cstdlib>
#include <iostream>
// #include <jsonbox.h>
#include <memory>
#include <restbed>
#include <sstream>
#include <string>

class WebServer {
  private:
	auto resource;
	int port;

  public:
	WebServer(int port);
	~WebServer();
	void operator()();
	void get_method_handler(const shared_ptr<Session> session);
};

// WebServer::WebServer(int port) : port(port) {}

WebServer::~WebServer() {}
