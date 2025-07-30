#include "web/webserver.hpp"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <iostream>
#include <mysql_connection.h>
#include <thread>

int main() {
	sql::Driver *driver = get_driver_instance();
	// sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	try {
		std::unique_ptr<sql::Connection> con(driver->connect("192.168.56.1:3306", "test", "test"));
		if (con != NULL) {
			std::cout << "MySQL Connected successfully!\n";
		}

		/*stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM test.test");
		// res = stmt->executeQuery("SELECT * FROM test");
		while (res->next()) {
			/*std::cout << "\t... MySQL replies: ";
			std::cout << res->getString("_message") << std::endl;
			std::cout << "\t... MySQL says it again: ";
			std::cout << res->getString(1) << std::endl; * /
			std::cout << "first: " << res->getInt("first") << "\nsecond: " << res->getInt("second") << std::endl;
		}*/
		/*delete res;
		delete stmt;
		delete con;*/

		// Prepared statement
		WebServer webserver(con.get(), 8080);
		std::thread webThread = std::thread(webserver);
		// ws->Test();
		std::cout << "Main: Webserver started on port 8080" << std::endl;
		webThread.join();

	} catch (sql::SQLException &e) {
		std::cerr << "MySQL was NOT started or Incorrect credentials.\n" << e.what() << std::endl;

		// closing all open connections
		/*if (con->isValid())
			con->close();
		if (driver->isRegistered())
			driver->threadEnd();
		*/
		return 1;
	}
}