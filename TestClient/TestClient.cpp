﻿// TestClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include "pch.h"
#include "utils.h"
#include "db.h"

#include "jdbc/mysql_connection.h"
#include "jdbc/mysql_driver.h"
#include "jdbc/mysql_error.h"
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/resultset.h>

#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <vector>
#include <exception>



using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

typedef  std::pair<utility::string_t, web::json::value> jsonvalue;
 

int main()
{

	
	while (true) {
		std::cout << "TEST" << std::endl << "1: feedback?rate=5";
		std::cout << "2: POST feedback " << std::endl;
		int test;
		std::cin >> test;
		auto fileStream = std::make_shared<ostream>();

		if (test == 1) {
			// Open stream to output file.
			pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
			{
				*fileStream = outFile;

				// Create http_client to send the request.
				http_client client(U("http://localhost:11111/api/feedback"));

				// Build request URI and start the request.
				uri_builder builder(U(""));
				builder.append_query(U("rate"), U("1"));
				return client.request(methods::GET, builder.to_string());
			})

				// Handle response headers arriving.
				.then([=](http_response response)
			{
				printf("Received response status code:%u\n", response.status_code());

				// Write response body into the file.
				return response.body().read_to_end(fileStream->streambuf());
			})

				// Close the file stream.
				.then([=](size_t)
			{
				return fileStream->close();
			});

			// Wait for all the outstanding I/O to complete and handle any exceptions
			try
			{
				requestTask.wait();
			}
			catch (const std::exception &e)
			{
				printf("Error exception:%s\n", e.what());
			}
		}
		else 		if (test == 2) {
			// Open stream to output file.
			pplx::task<void> requestTask2 = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
			{
				*fileStream = outFile;
				
				
				// Create http_client to send the request.
				http_client client(U("http://localhost:34568/api/feedback/"));
				
			
				utility::stringstream_t ss;
				int rate = 4;
				utility::string_t comment = U("Foarte tare;");
				ss << "{ \"rate\" : " << rate << ", \"comment\" : \"" << comment << "\"}";
				std::wcout << ss.str() << std::endl;
				json::value objRate = json::value::parse(ss.str());

				utility::ostringstream_t buf;
			
				uri_builder builder(U("/1"));
				http_request msg(methods::POST);
				msg.headers().add(U("Ubi-UserId"), U("ionut"));
				
				msg.set_request_uri(builder.to_string());
				msg.set_body(objRate);
				//response["ionut"] = json::value::number(1);
				//response["status”] = json::value::string("ready!”);
				// Build request URI and start the request.

			
				return client.request(msg);
			})

				// Handle response headers arriving.
				.then([=](http_response response)
			{
				printf("Received response status code:%u\n", response.status_code());

				// Write response body into the file.
				return response.body().read_to_end(fileStream->streambuf());
			})

				// Close the file stream.
				.then([=](size_t)
			{
				return fileStream->close();
			});

			// Wait for all the outstanding I/O to complete and handle any exceptions
			try
			{
				requestTask2.wait();
			}
			catch (const std::exception &e)
			{
				printf("Error exception:%s\n", e.what());
			}
		}

	}
}

