#pragma once
#include <vector>
#include "jdbc/mysql_connection.h"
#include "jdbc/mysql_driver.h"
#include "jdbc/mysql_error.h"
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/resultset.h>

struct feedback {
	 int id;
	 std::string userid;
	 int sessionid;
	 int rate;
	 std::string comment;
};
class DB {
public:
	DB() {
		server = "192.168.2.104";
		username = "test";
		password = "test";
		schema = "test";

	}
	DB(const std::string& m_server  , const std::string m_username, const std::string m_pass,const std::string m_schema) :
		server(m_server),username(m_username),password(m_pass),schema(m_schema) {

	}

	bool insertFeedback(const feedback &feedback);
	const std::vector<feedback> * getFeedback(const int rate);

	sql::Connection* initConnections();

	 std::string server;
	 std::string username;
	 std::string password;
	 std::string schema;
};