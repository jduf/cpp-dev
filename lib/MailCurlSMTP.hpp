#ifndef DEF_MAILCURLSMTP
#define DEF_MAILCURLSMTP

#include "IOFiles.hpp"
#include <curl/curl.h>

class MailCurlSMTP{
	public:
		MailCurlSMTP();
		~MailCurlSMTP();
		/*{Forbidden*/
		MailCurlSMTP(MailCurlSMTP const&) = delete;
		MailCurlSMTP(MailCurlSMTP&&) = delete;
		MailCurlSMTP& operator=(MailCurlSMTP) = delete;
		/*}*/

		void save_account(IOFiles& w) const;
		void set_account(IOFiles& r);
		void set_account(std::string const& from, std::string const& signature, std::string const& password, std::string const& url);

		void set_to(std::string const& to = ""){ to_ = to; }
		void set_subject(std::string const& subject = ""){ subject_ = subject; }
		void set_body(std::string const& body = ""){ body_ = body; }
		void set_pdf(std::string const& pdf = ""){ pdf_ = pdf; }
		void set_log(std::string const& logname);

		void send();

		std::string const& get_from() const { return from_; }
		std::string const& get_signature() const { return signature_; }
		std::string const& get_url() const { return url_; }

	private:
		std::string from_ = "";
		std::string signature_ = "";
		std::string password_ = "";
		std::string url_ = "";
		std::string to_ = "";
		std::string subject_ = "";
		std::string body_ = "";
		std::string pdf_ = "";

		FILE* log_ = NULL;
		CURL* curl_ = NULL;
		CURLcode ret_ = CURLE_OK;
};
#endif
