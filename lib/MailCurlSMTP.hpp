#ifndef DEF_MAILCURLSMTP
#define DEF_MAILCURLSMTP

#include <string>
#include <curl/curl.h>

class MailCurlSMTP{
	public:
		MailCurlSMTP(std::string const& nameFrom, std::string const& from);
		~MailCurlSMTP();

		CURLcode send(std::string const & to, std::string const & subject, std::string const & body);


		//void set_from(const std::string& from);
		//void set_password(const std::string& password);
		//void set_to(const std::vector<std::string>& to);
		//void set_secret(const std::vector<std::string>& secret);
		//void set_cc(const std::vector<std::string>& cc);
		//void set_attach(const std::vector<std::string>& attach);
		//void set_subject(const std::string& subject);
		//void set_message(const std::string& message);
		//void set_server(const std::string& server);
		//void set_port(const std::string& port);
//
	private:
		std::string const from_;
		std::string const nameFrom_;
		//std::vector<std::string> send_buffer_;
		//std::vector<std::string> to_;
		//std::vector<std::string> cc_;

		CURL *curl_;
		CURLcode ret_;

		//void set_receiver_list();
		//void set_curl_option();
		//void make_send_message();
		//void set_receiver_list();
};
#endif
