#include"pch.h"
#include <stdio.h>

#include <iostream>
#include <exception>

/***
 * Copyright (C) Microsoft. All rights reserved.
 * Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
 *
 * =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *
 * BlackJack_Servr.cpp - Simple server application for blackjack
 *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 ****/

#include <cpprest/http_listener.h>
#include "utils.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class ClientRestApi : public HTTPEndpoint{
public:
	ClientRestApi():HTTPEndpoint(){}
	ClientRestApi(const string_t& url) :HTTPEndpoint(url) {}
private :
	void handle_post(http_request message);
};
std::unique_ptr<ClientRestApi> g_httpDealer;

void ClientRestApi::handle_post(http_request message) {
	

	std::cout << "handle post" << std::endl;
	if (message.headers().has(U("Ubi-UserId"))) {
		auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));
		unsigned int sessionId = 0;
		if (paths.size() > 1) {
			if (paths[0] == U("feedback"))
			{
				sessionId = std::stoi(paths[1]);
			}
		}
		if (sessionId == 0) {
			//TODO
		}
		auto itr = message.headers().find(U("Ubi-UserId"));
		string_t userID = itr->second;
		int rate = 0;
		string_t comment(U(""));
		json::value response = message.extract_json().get();
		if (response.is_object()) {
			rate = response.at(U("rate")).as_integer();
			comment = response.at(U("comment")).as_string();

			std::wcout << "rate:" << response.at(U("rate")).as_integer() << ", ";
			std::wcout << "comment" << response.at(U("comment")).as_string() << std::endl;
		}
		feedback feed;
		feed.userid = utility::conversions::to_utf8string(userID);
		feed.rate = rate;
		feed.sessionid = sessionId;
		feed.comment = utility::conversions::to_utf8string(comment);
		db.insertFeedback(feed);



		//std::cout << response.serialize();
		message.reply(status_codes::OK);
	}
	
}

void on_initialize(const string_t& address)
{
	// Build our listener's URI from the configured address and the hard-coded path "blackjack/dealer"
	utility::string_t custome = U("/api/");
	uri_builder uri(address);
	uri.append_path(custome);

	auto addr = uri.to_uri().to_string();
	g_httpDealer = std::unique_ptr<ClientRestApi>(new ClientRestApi(addr));
	g_httpDealer->open().wait();

	ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

	return;
}

void on_shutdown()
{
	g_httpDealer->close().wait();
	return;
}

//
// To start the server, run the below command with admin privileges:
// BlackJack_Server.exe <port>
// If port is not specified, will listen on 34568
//

int main(int argc, char* argv[])

{
	utility::string_t port = U("34568");


	utility::string_t address = U("http://localhost:");
	address.append(port);

	on_initialize(address);
	std::cout << "Press ENTER to exit." << std::endl;

	std::string line;
	std::getline(std::cin, line);

	on_shutdown();
	return 0;
}