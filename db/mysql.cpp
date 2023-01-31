#include "mysql.hpp"

#include <cppconn/driver.h>

namespace mysql
{
	void server::connect()
	{
		if (!driver) {
			driver = get_driver_instance();
		}

		if (!connection) {
			connection = driver->connect(server_url, username, password);
		}
		else if (connection->isClosed()) {
			connection->reconnect();
		}

		connection->setSchema(database);

	}

}
