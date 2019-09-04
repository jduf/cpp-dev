#include "MailCurlSMTP.hpp"

int main(){
	MailCurlSMTP mail("Jérôme Gmail","jerome.dufour.karate@gmail.com");
	mail.send("jejedufour@yahoo.fr","sujet 1","text principal");
}
