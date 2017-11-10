#ifndef DEF_TP
#define DEF_TP

#include "ArrayOfStrings.hpp"
#include "Gnuplot.hpp"
#include "Parseur.hpp"

class TP{
	public:
		TP(Parseur& P);
		TP(IOFiles& ftp);
		TP() = delete;
		~TP() = default;

		void analyse();
		void display();
		void save();
		void add();
		void edit(unsigned int student=-1);
		void histogram();
		void summary();
		void feedback();
		void pdf(){
			summary();
			feedback();
		}
		void clean();

	private:
		std::string class_id_;
		unsigned int TP_number_;
		unsigned int nstudents_;
		ArrayOfStrings comments_;
		Matrix<double> points_;
		Vector<double> max_points_;
		Vector<double> grades_;
		std::vector<std::string> class_list_;

		double average_;
		unsigned int nfails_;

		std::string const criteria_[9] = {"Table des matières", "Notice principale", "Notices TP", "Numérotation", "Préparation", "Questions préliminaires", "Schéma", "Unités", "Compte rendu"};
};
#endif
