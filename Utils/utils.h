#pragma once

#include <cpprest/http_listener.h>
#include "db.h"
 
class Utils {
public:
	Utils();
	utility::string_t DisplayJSONValue(web::json::value v);

};

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

typedef  std::pair<utility::string_t, web::json::value> jsonvalue;


class HTTPEndpoint : public Utils
{
public:
	HTTPEndpoint() :
		db("192.168.2.104", "test", "test", "test"){

	}
	HTTPEndpoint(utility::string_t url);

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }
	DB db;



private:
	virtual void handle_get(http_request message);
	virtual void handle_put(http_request message);
	virtual void handle_post(http_request message);
	virtual void handle_delete(http_request message);

	http_listener m_listener;
};

