#include "webserver.h"

/*void webserver::HandleClient(socket socket) {
	/*boost::system::error_code ec;

	// Lesen der Daten vom Client
	// werden Daten aus dem Socket gelesen und im globalen Puffer vBuffer
	// gespeichert.
	size_t length = socket.read_some(boost::asio::buffer(vBuffer.data(), vBuffer.size()), ec);
	if (ec) // Falls ein Fehler auftritt (ec ist nicht leer), wird die
			// Fehlermeldung ausgegeben.
	{
		std::cerr << "Error reading from socket: " << ec.message() << std::endl;
	} else // Wenn Daten erfolgreich gelesen wurden, werden sie auf der Konsole
		   // ausgegeben.
	{
		std::cout << "\nReceived data (" << length << " bytes):\n";
		std::cout.write(vBuffer.data(), length);
		std::cout << "\n";

		// Sende eine Antwort zurück an den Client
		std::string response = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\nHello from server!";
		boost::asio::write(socket, boost::asio::buffer(response), ec);
	}* /

	listen(serverSocket, 5);

	int clientSocket = accept(serverSocket, NULL, NULL);

	std::cout << "Client connected" << std::endl;
} */

void WebServer::Test() {
	std::cout << "Test" << std::endl;
}