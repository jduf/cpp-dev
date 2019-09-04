#include "MailCurlSMTP.hpp"

MailCurlSMTP::MailCurlSMTP(std::string const& from, std::string const& password, std::string const& url):
	from_(from),
	password_(password),
	url_(url),
	ret_(CURLE_OK)
{ curl_global_init(CURL_GLOBAL_ALL); }

MailCurlSMTP::~MailCurlSMTP(){ curl_global_cleanup(); }

CURLcode MailCurlSMTP::send(std::string const& to, std::string const& subject){
	curl_ = curl_easy_init();
	if(curl_){
		struct curl_slist *headers(NULL);
		struct curl_slist *recipients(NULL);
		curl_mime *mime;
		curl_mimepart *part;

		curl_easy_setopt(curl_, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl_, CURLOPT_URL, url_.c_str());
		curl_easy_setopt(curl_, CURLOPT_MAIL_FROM, from_.c_str());
		recipients = curl_slist_append(recipients,to.c_str());
		curl_easy_setopt(curl_, CURLOPT_MAIL_RCPT, recipients);
		curl_easy_setopt(curl_, CURLOPT_USERPWD, (from_ + ":" + password_).c_str());

		time_t tt;
		time(&tt);
		char timestring[128] = {0};
		strftime(timestring, 127,"Date: %d %b %y %H:%M:%S %Z", localtime(&tt));

		headers = curl_slist_append(NULL,  timestring);
		headers = curl_slist_append(headers,("From: <" + from_ + "> ").c_str());
		headers = curl_slist_append(headers,("To: <" + to + ">").c_str());
		headers = curl_slist_append(headers,("Subject: " + subject).c_str());
		headers = curl_slist_append(headers, "User-Agent: libcurl");
		headers = curl_slist_append(headers, "MIME-Version: 1.0");
		headers = curl_slist_append(headers, "Content-Type: multipart/mixed;");
		headers = curl_slist_append(headers, "Content-Type: text/plain; charset=utf-8; format=flowed");
		headers = curl_slist_append(headers, "Content-Transfer-Encoding: 7bit");
		curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

		mime = curl_mime_init(curl_);

		part = curl_mime_addpart(mime);
		curl_mime_data(part, body_.c_str(), CURL_ZERO_TERMINATED);
		curl_easy_setopt(curl_, CURLOPT_MIMEPOST, mime);

		part = curl_mime_addpart(mime);
		curl_mime_filedata(part, "tmp.pdf");
		curl_mime_encoder(part, "base64");
		curl_easy_setopt(curl_, CURLOPT_MIMEPOST, mime);

		ret_ = curl_easy_perform(curl_);
		if(ret_ != CURLE_OK){
			std::cerr<<"curl_easy_perform() failed ("<<ret_<<"): "<<curl_easy_strerror(ret_)<<std::endl;
		}

		curl_slist_free_all(recipients);
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl_);
		curl_mime_free(mime);
	}
	return ret_;
}
