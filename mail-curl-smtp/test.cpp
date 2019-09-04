#include "MailCurlSMTP.hpp"

int main(){
	std::string password;
	std::cout << "Enter pass\n";
	std::cin>>password;
	
	MailCurlSMTP mail("jerome.dufour@vd.educanet2.ch",password,"smtps://mail.educanet2.ch");
	mail.set_body(
			"Bonjour,\r\n"
			"Voici l'évaluation de votre rapport de TP.\r\n"
			"Meilleures salutations,\r\n"
			"Jérôme Dufour");
	mail.send("jejedufour@yahoo.fr","sujet 1");

	mail.set_body(
			"Bonjour,\r\n"
			"Voici l'évaluation de votre travail.\r\n"
			"Meilleures salutations,\r\n"
			"Jérôme Dufour");
	mail.send("jerome.dufour.karate@gmail.com","sujet 2");
}
