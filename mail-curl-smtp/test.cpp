#include "MailCurlSMTP.hpp"

int main(){
	MailCurlSMTP mail;

	IOFiles r("param.jdbin",false,false);
	mail.set_account(r);

	//std::string password;
	//std::cout << "Enter password\n";
	//std::cin>>password;
	//MailCurlSMTP mail("jerome.dufour@vd.educanet2.ch",password,"smtps://mail.educanet2.ch");

	mail.set_body(
			"Bonjour,\r\n"
			"Voici l'évaluation de votre rapport de TP.\r\n"
			"Meilleures salutations,\r\n"
			"Jérôme Dufour");
	//mail.set_pdf("tmp.pdf");
	mail.set_subject("sujet");
	mail.set_log("m1.log");
	mail.set_to("jerome.dufour.plongee@gmail.com");
	//mail.send();

	mail.set_log("m2.log");
	mail.set_to("jerome.dufour.karate@gmail.com");
	//mail.send();

	//IOFiles w("param.jdbin",true,false);
	//mail.save_account(w);
}
