#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <vector>
#include <thread>

//Es wird die Boost.Asio-Bibliothek eingebunden
//Ein globaler Puffer vBuffer wird erstellt. Der Puffer ist 1024 Bytes groß.
std::vector<char> vBuffer(1024); // Puffer zum Speichern von empfangenen Daten

//Die Funktion nimmt einen socket entgegen, der eine Verbindung zum Client repräsentiert.
void HandleClient(boost::asio::ip::tcp::socket socket)
{
    boost::system::error_code ec;

    // Lesen der Daten vom Client
    //werden Daten aus dem Socket gelesen und im globalen Puffer vBuffer gespeichert.
    size_t length = socket.read_some(boost::asio::buffer(vBuffer.data(), vBuffer.size()), ec);
    if (ec) //Falls ein Fehler auftritt (ec ist nicht leer), wird die Fehlermeldung ausgegeben.
        {
        std::cerr << "Error reading from socket: " << ec.message() << std::endl;
    } else  //Wenn Daten erfolgreich gelesen wurden, werden sie auf der Konsole ausgegeben.
        {
        std::cout << "\nReceived data (" << length << " bytes):\n";
        std::cout.write(vBuffer.data(), length);
        std::cout << "\n";

        // Sende eine Antwort zurück an den Client
        std::string response = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\nHello from server!";
        boost::asio::write(socket, boost::asio::buffer(response), ec);
    }
}

int main() {
    try {
        //Zentrale Komponente, die alle asynchronen Operationen verwaltet.
        boost::asio::io_context context;

        // Erstellen eines Endpunkts für den Server (Port 8080)
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 8080);

        // Erstellen eines Acceptors, um eingehende Verbindungen zu akzeptieren
        //Wartet auf eingehende Verbindungen. Der Server ist so konfiguriert, dass er auf Port 8080 lauscht.
        boost::asio::ip::tcp::acceptor acceptor(context, endpoint);
        std::cout << "Server listening on port 8080...\n";

        while (true)    //Der Server bleibt aktiv und akzeptiert kontinuierlich neue Verbindungen.
            {
            // Warten auf eine eingehende Verbindung
            boost::asio::ip::tcp::socket socket(context);
            // Blockiert, bis ein Client eine Verbindung herstellt.
            acceptor.accept(socket);

            std::cout << "Client connected!\n";

            // Für jeden verbundenen Client wird ein separater Thread gestartet,
            // um die Kommunikation abzuwickeln (HandleClient)
            std::thread(HandleClient, std::move(socket)).detach();
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
