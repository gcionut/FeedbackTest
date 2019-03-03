#include "stdafx.h"
#include "utils.h"

Utils::Utils() {

}
utility::string_t Utils::DisplayJSONValue(web::json::value v)
{
	utility::stringstream_t ss;
	try
	{
		if (!v.is_null())
		{
			if (v.is_object())
			{
				// Loop over each element in the object
				for (auto iter = v.as_object().cbegin(); iter != v.as_object().cend(); ++iter)
				{
					// It is necessary to make sure that you get the value as const reference
					// in order to avoid copying the whole JSON value recursively (too expensive for nested objects)
					const utility::string_t &str = iter->first;
					const web::json::value &value = iter->second;

					if (value.is_object() || value.is_array())
					{
						ss << "Parent: " << str << std::endl;

						ss << DisplayJSONValue(value);

						ss << "End of Parent: " << str << std::endl;
					}
					else
					{
						ss << "str: " << str << ", Value: " << value.serialize() << std::endl;
					}
				}
			}
			else if (v.is_array())
			{
				// Loop over each element in the array
				for (size_t index = 0; index < v.as_array().size(); ++index)
				{
					const web::json::value &value = v.as_array().at(index);

					ss << "Array: " << index << std::endl;
					ss << DisplayJSONValue(value);
				}
			}
			else
			{
				ss << "Value: " << v.serialize() << std::endl;
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		ss << "Value: " << v.serialize() << std::endl;
	}

	return ss.str();
}


HTTPEndpoint::HTTPEndpoint(utility::string_t url) : m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&HTTPEndpoint::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&HTTPEndpoint::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&HTTPEndpoint::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&HTTPEndpoint::handle_delete, this, std::placeholders::_1));



}
void HTTPEndpoint::handle_get(http_request message) {
	message.reply(status_codes::BadRequest);
}
void HTTPEndpoint::handle_post(http_request message) {
	message.reply(status_codes::BadRequest);

}
void HTTPEndpoint::handle_put(http_request message) {
	message.reply(status_codes::BadRequest);

}
void HTTPEndpoint::handle_delete(http_request message) {
	message.reply(status_codes::BadRequest);

}