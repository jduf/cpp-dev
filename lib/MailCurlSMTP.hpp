#ifndef DEF_MAILCURLSMTP
#define DEF_MAILCURLSMTP

#include "Rand.hpp"
#include <iostream>
#include <string>
#include <curl/curl.h>

class MailCurlSMTP{
	public:
		MailCurlSMTP(std::string const& from, std::string const& password, std::string const& url);
		~MailCurlSMTP();

		CURLcode send(std::string const & to, std::string const & subject);
		void set_body(std::string const& body){ body_ = body; }

	private:
		std::string from_;
		std::string password_;
		std::string url_;
		std::string body_;

		CURL *curl_;
		CURLcode ret_;
};
#endif
