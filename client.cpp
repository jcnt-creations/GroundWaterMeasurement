#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <vector>

int main() {
    try {
        //Der IO-Kontext des Clients.
        boost::asio::io_context context;

        // Erstellen des Endpunkts für den Server (Server-Adresse und Port)
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("127.0.0.1"), 8080); // Server läuft auf localhost und Port 8080

        // Erstellen eines Sockets
        boost::asio::ip::tcp::socket socket(context);

        // Verbindung zum Server herstellen
        socket.connect(endpoint);

        std::cout << "Connected to server!" << std::endl;

        // Anfrage an den Server senden
        std::string request = "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
        boost::asio::write(socket, boost::asio::buffer(request));

        // Puffer für die Antwort des Servers
        std::vector<char> buffer(1024);

        // Antwort vom Server lesen
        boost::system::error_code ec;
        size_t length = socket.read_some(boost::asio::buffer(buffer), ec);

        if (!ec) {
            std::cout << "Server response (" << length << " bytes):\n";
            std::cout.write(buffer.data(), length); // Die Antwort des Servers ausgeben
        } else {
            std::cerr << "Error reading from server: " << ec.message() << std::endl;
        }

        // Verbindung schließen
        socket.close();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
