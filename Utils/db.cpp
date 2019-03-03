#include "stdafx.h"

#include <iostream>
#include <exception>

#include "db.h"
#include <jdbc/cppconn/prepared_statement.h>

//#include "mysql_connection.h"

sql::Connection* DB::initConnections() {
	try {
		sql::mysql::MySQL_Driver *driver;
		driver = sql::mysql::get_mysql_driver_instance();
		sql::Connection *con;
		std::string hostName = "tcp://" + server + ":3306;";
		con = driver->connect(hostName.c_str(), username.c_str(), password.c_str());
		con->setSchema(schema.c_str());
		return con;
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
	return NULL;
}

bool DB::insertFeedback(const feedback & feedback)
{
	sql::Connection *con = initConnections();
	sql::PreparedStatement  *prep_stmt = NULL;
	bool resp = true;
	try {
		if (con != NULL) {
			// ...

			prep_stmt = con->prepareStatement("INSERT INTO feedback( userid,idsession,rate,feedback) VALUES ( ?,?,?,?)");

			prep_stmt->setString(1, feedback.userid.c_str());
			prep_stmt->setInt(2, feedback.sessionid);
			prep_stmt->setInt(3, feedback.rate);
			prep_stmt->setString(4, feedback.comment.c_str());
			bool resp  = prep_stmt->execute();
			
		}
	}
	catch(std::exception& ex) {
		std::cout << ex.what() << std::endl ;
		resp = false;
	}
	delete prep_stmt;
	delete con;

	return resp;
}

const  std::vector<feedback>* DB::getFeedback(const int rate)
{
	sql::Connection *con = initConnections();
	sql::ResultSet *res = NULL;
	sql::PreparedStatement  *prep_stmt = NULL;
	std::vector<feedback> *arr = new std::vector<feedback>();
	try {
		if (con != NULL) {
			// ...

			prep_stmt = con->prepareStatement("select * from feedback where rate = ? limit 15");

			
			prep_stmt->setInt(1, rate);

			res  = prep_stmt->executeQuery();
			while (res->next()){
				feedback feed = {
				res->getInt("idfeedback"),
				res->getString(2).c_str(),
				res->getInt(3),
				res->getInt(4),
				res->getString(5).c_str()
				};
				// You can use either numeric offsets...
				std::cout << "id = " << feed.id; // getInt(1) returns the first column
				// ... or column names for accessing results.
				// The latter is recommended.
				arr->push_back(feed);
			}
			
		}
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
		
	}
	delete prep_stmt;
	delete con;
	delete res;

	return arr;
	// TODO: insert return statement here
}
