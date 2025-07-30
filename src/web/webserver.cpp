#include "webserver.hpp"

WebServer::WebServer(int port) : port(port) {
	resource = make_shared<Resource>();
	resource->set_path("/resource");
	resource->set_method_handler("GET", bind(&WebServer::get_method_handler, this, placeholders::_1));

	auto settings = make_shared<Settings>();
	settings->set_port(1984);
	settings->set_default_header("Connection", "close");

	Service service;
	service.publish(resource);
	service.start(settings);
}

void WebServer::get_method_handler(const shared_ptr<Session> session) {
	const auto request = session->get_request();
	const auto response = session->get_response();

	const auto query_parameters = request->get_query_parameters();
	const auto name = query_parameters->get("name", "World");

	stringstream stream;
	stream << "Hello, " << name << "!";

	const auto message = stream.str();

	response->set_status(200);
	response->set_header("Content-Type", "text/plain");
	response->set_body(message);
	session->close(move(response));

	/*const auto request = session->get_request();

	size_t content_length = request->get_header("Content-Length", 0);

	session->fetch(content_length, [](const shared_ptr<Session> &session, const Bytes &body) {
		JsonBox::Value json;
		json.loadFromString(string(body.begin(), body.end()));

		// perform awesome solutions logic...

		stringstream stream;
		json.writeToStream(stream);
		string response_body = stream.str();

		session->close( OK, response_body, { { "Content-Length", ::to_string( response_body.length( ) }, { "Content-Type": "application/json" } } );
	});*/
}