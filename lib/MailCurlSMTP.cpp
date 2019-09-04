#include <iostream>
#include "MailCurlSMTP.hpp"
#include "Rand.hpp"

MailCurlSMTP::MailCurlSMTP(std::string const& nameFrom, std::string const& from):
	from_(from),
	nameFrom_(nameFrom),
	ret_(CURLE_OK)
{
	curl_global_init(CURL_GLOBAL_ALL);
}

MailCurlSMTP::~MailCurlSMTP(){
	curl_global_cleanup();
}

CURLcode MailCurlSMTP::send(std::string const& to, std::string const& subject, std::string const& body){
	//set_receiver_list();
	//make_send_message();
	//set_curl_option();

	curl_ = curl_easy_init();
	if(curl_){
		struct curl_slist *headers(NULL);
		struct curl_slist *recipients(NULL);
		//struct curl_slist *slist(NULL);
		curl_mime *mime;
		curl_mimepart *part;

		curl_easy_setopt(curl_, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl_, CURLOPT_URL, "smtps://smtp.gmail.com");
		curl_easy_setopt(curl_, CURLOPT_MAIL_FROM, from_.c_str());
		recipients = curl_slist_append(recipients,to.c_str());
		curl_easy_setopt(curl_, CURLOPT_MAIL_RCPT, recipients);

		/* Set smtp password */
		curl_easy_setopt(curl_, CURLOPT_USERPWD, "jerome.dufour.karate@gmail.com:");

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

		/* Text message. */
		part = curl_mime_addpart(mime);
		curl_mime_data(part, body.c_str(), CURL_ZERO_TERMINATED);
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

//void CurlSmtp::set_curl_option()
//{
	//pooh_.pos = 0;
	//pooh_.counter = 0;
	//pooh_.data.resize(send_buffer_.size() + 1);
	//pooh_.data.insert(pooh_.data.begin(), send_buffer_.begin(), send_buffer_.end());
//
//
	//curl_easy_setopt(curl_, CURLOPT_READFUNCTION, read_callback);
	//curl_easy_setopt(curl_, CURLOPT_READDATA, &pooh_);
//}
//
//
//void MailCurlSMTP::set_receiver_list(){
	//for (int i = 0; i < to_.size(); i++){
		//rcpt_list_ = curl_slist_append(rcpt_list_, std::string(LEFT_BRACE + to_[i] + RIGTH_BRACE).c_str());
	//}
	//for (int i = 0; i < cc_.size(); i++){
		//rcpt_list_ = curl_slist_append(rcpt_list_, std::string(LEFT_BRACE + cc_[i] + RIGTH_BRACE).c_str());
	//}
	//for (int i = 0; i < secret_.size(); i++){
		//rcpt_list_ = curl_slist_append(rcpt_list_, std::string(LEFT_BRACE + secret_[i] + RIGTH_BRACE).c_str());
	//}
//}
//
//void MailCurlSMTP::make_send_message(){
	//send_buffer_.clear();
	//// time
	//time_t t;
	//time(&t);
	//char timestring[128] = {0};
	//std::string timeformat = "Date: %d %b %y %H:%M:%S %Z";
	//if (strftime(timestring, 127, timeformat.c_str(), localtime(&t)))
	//{ // got the date
		//send_buffer_.push_back(timestring);
	//}
//
	//// from
	//send_buffer_.push_back("From: " LEFT_BRACE + from_ + RIGTH_BRACE);
//
	//// to
	//for (int i = 0; i < to_.size(); ++i)
	//{
		//send_buffer_.push_back("To: " LEFT_BRACE + to_[i] + RIGTH_BRACE);
	//}
	//// cc
	//for (int i = 0; i < cc_.size(); ++i)
	//{
		//send_buffer_.push_back("Cc: " LEFT_BRACE + cc_[i] + RIGTH_BRACE);
	//}
//
	//// subject
	//send_buffer_.push_back("Subject: " + subject_);
//
	//if (attach_.empty())
	//{
		//// split body
		//send_buffer_.push_back(ENTER);
		//// message
		//send_buffer_.push_back(message_ + ENTER);
	//}
	//else
	//{
		//// user agent
		//send_buffer_.push_back(USER_AGENT);
		//send_buffer_.push_back(MIME_VER);
		//send_buffer_.push_back(HEADER_CONTENT_TYPE);
//
		//std::string boundary(get_boundary());
		//// set boundary
		//send_buffer_.push_back(" boundary=\"" + boundary + "\"" ENTER);
//
		//// first part of body, boundary header and message
		//send_buffer_.push_back(BOUNDARY_FLAG + boundary);
		//send_buffer_.push_back(MSG_CONTENT_TYPE);
		//send_buffer_.push_back(MSG_ENCODING);
//
		//// split body
		//send_buffer_.push_back(ENTER);
//
		//send_buffer_.push_back(message_ + ENTER);
		//send_buffer_.push_back(BOUNDARY_FLAG + boundary);
//
		//// attachment
		//for (int i = 0; i < attach_.size(); ++i)
		//{
			//attach(attach_[i]);
		//}
		//for (std::vector<std::pair<std::vector<char>, std::string>>::iterator it1 = attachment_.begin();
				//it1 != attachment_.end(); ++it1)
		//{
			//if (it1->second.length() > 3)
			//{ // long enough for an extension
				//std::string typ(it1->second.substr(it1->second.length() - 4, 4));
//
				//if (typeMap_.count(typ) > 0)
				//{
					//send_buffer_.push_back(typeMap_[typ]);
				//}
				//else
				//{ // add other types
					//// everything else
					//send_buffer_.push_back("Content-Type: application/X-other-1;");
				//}
			//}
			//else
			//{
				//// default to don't know
				//send_buffer_.push_back("Content-Type: application/X-other-1;");
			//}
//
			//send_buffer_.push_back(" name=\"" + it1->second + "\"");
			//send_buffer_.push_back("Content-Transfer-Encoding: base64");
			//send_buffer_.push_back("Content-Disposition: attachment; filename=\"" + it1->second + "\"");
//
			//// split body
			//send_buffer_.push_back(ENTER);
//
			//send_buffer_.push_back(std::string(it1->first.begin(), it1->first.end()));
			//// terminate the message with the boundary + "--"
			//if ((it1 + 1) == attachment_.end())
				//send_buffer_.push_back(BOUNDARY_FLAG + boundary + BOUNDARY_FLAG);
			//else
				//send_buffer_.push_back(BOUNDARY_FLAG + boundary);
		//}
	//}
//
	//// add \r\n to each item
	//for (int i = 0; i < send_buffer_.size(); ++i)
	//{
		//send_buffer_[i] += ENTER;
	//}
//}
