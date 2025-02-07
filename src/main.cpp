#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <vector>

// Es wird die Boost.Asio-Bibliothek eingebunden
// Ein globaler Puffer vBuffer wird erstellt. Der Puffer ist 1024 Bytes groß.
std::vector<char> vBuffer(1024); // Puffer zum Speichern von empfangenen Daten

// Die Funktion nimmt einen socket entgegen, der eine Verbindung zum Client
// repräsentiert.
/*void HandleClient(boost::asio::ip::tcp::socket socket) {
    boost::system::error_code ec;

    // Lesen der Daten vom Client
    // werden Daten aus dem Socket gelesen und im globalen Puffer vBuffer
    // gespeichert.
    size_t length = socket.read_some(
        boost::asio::buffer(vBuffer.data(), vBuffer.size()), ec);
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
        std::string response =
            "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\nHello from server!";
        boost::asio::write(socket, boost::asio::buffer(response), ec);
    }
}*/

int main() {
    try {
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}