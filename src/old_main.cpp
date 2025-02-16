// #include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <mysql_driver.h>
// #include <netinet/in.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <string.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

// Es wird die Boost.Asio-Bibliothek eingebunden
// Ein globaler Puffer vBuffer wird erstellt. Der Puffer ist 1024 Bytes
// groß.
// std::vector<char> vBuffer(1024); // Puffer zum Speichern von empfangenen
// Daten

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

void connectToDatabase() {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    try {

        // Create a connection
        driver = sql::mysql::get_mysql_driver_instance();
        /*con = driver->connect("tcp://127.0.0.1:3306", "root");

        // Connect to the MySQL test database
        con->setSchema("test");

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
        while (res->next()) {
            std::cout << "\t... MySQL replies: ";
            std::cout << res->getString("_message") << std::endl;
            std::cout << "\t... MySQL says it again: ";
            std::cout << res->getString(1) << std::endl;
        }
        delete res;
        delete stmt;
        delete con;
        */

    } catch (sql::SQLException &e) {
        std::cerr << "# ERR: SQLException in " << __FILE__;
        std::cerr << "(" << __FUNCTION__ << ") on line " << __LINE__
                  << std::endl;
        std::cerr << "# ERR: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
}

int old_main() {
    try {
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        std::cout << "Server started" << std::endl;
        connectToDatabase();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}