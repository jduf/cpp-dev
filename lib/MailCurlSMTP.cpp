#include "MailCurlSMTP.hpp"

MailCurlSMTP::MailCurlSMTP()
{ curl_global_init(CURL_GLOBAL_ALL); }

MailCurlSMTP::~MailCurlSMTP(){ 
	curl_global_cleanup(); 
	if(log_){ fclose(log_); }
}

void MailCurlSMTP::set_account(std::string const& from, std::string const& signature, std::string const& password, std::string const& url){
	from_ = from;
	signature_ = signature;
	password_ = password;
	url_ = url;
}

void MailCurlSMTP::set_account(IOFiles& r){
	from_ = my::hex2string(r.read<std::string>());
	signature_ = my::hex2string(r.read<std::string>());
	password_ = my::hex2string(r.read<std::string>());
	url_ = my::hex2string(r.read<std::string>());
}

void MailCurlSMTP::save_account(IOFiles& w) const {
	w<<my::string2hex(from_)<<my::string2hex(signature_)<<my::string2hex(password_)<<my::string2hex(url_);
}

void MailCurlSMTP::set_log(std::string const& logname){
	if(log_){ fclose(log_); }
	log_ = fopen(logname.c_str(), "w"); 
}

void MailCurlSMTP::send(){
	curl_ = curl_easy_init();
	if(curl_){
		struct curl_slist* headers(NULL);
		struct curl_slist* recipients(NULL);
		curl_mime* mime;
		curl_mimepart* part;

		curl_easy_setopt(curl_, CURLOPT_VERBOSE, 1L);
		if(log_){
			curl_easy_setopt(curl_, CURLOPT_STDERR, log_);
			fprintf(log_, (to_  + "\n").c_str());
			fprintf(log_, (subject_ + "\n").c_str());
			fprintf(log_, (body_ + "\n").c_str());
			fprintf(log_, (pdf_ + "\n").c_str());
		}
		curl_easy_setopt(curl_, CURLOPT_URL, url_.c_str());
		curl_easy_setopt(curl_, CURLOPT_MAIL_FROM, from_.c_str());
		recipients = curl_slist_append(recipients,to_.c_str());
		curl_easy_setopt(curl_, CURLOPT_MAIL_RCPT, recipients);
		curl_easy_setopt(curl_, CURLOPT_USERPWD, (from_ + ":" + password_).c_str());

		time_t tt;
		time(&tt);
		char timestring[128];
		strftime(timestring, 127,"Date: %d %b %y %H:%M:%S %Z", localtime(&tt));

		headers = curl_slist_append(NULL,  timestring);
		headers = curl_slist_append(headers,("From: <" + from_ + "> ").c_str());
		headers = curl_slist_append(headers,("To: <" + to_ + ">").c_str());
		headers = curl_slist_append(headers,("Subject: " + subject_).c_str());
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

		if(pdf_ != ""){
			part = curl_mime_addpart(mime);
			curl_mime_filedata(part, pdf_.c_str());
			curl_mime_encoder(part, "base64");
			curl_easy_setopt(curl_, CURLOPT_MIMEPOST, mime);
		}

		ret_ = curl_easy_perform(curl_);
		if(ret_ != CURLE_OK){
			std::cerr<<"curl_easy_perform() failed ("<<ret_<<"): "<<curl_easy_strerror(ret_)<<std::endl;
		}

		curl_slist_free_all(recipients);
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl_);
		curl_mime_free(mime);
	}
}
