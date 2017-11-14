#ifndef DEF_EXAMEN
#define DEF_EXAMEN

#include "Note.hpp"

class Examen: public Note{
	public:
		Examen(Parseur& P);
		Examen(IOFiles& fexa);
		Examen() = delete;
		~Examen() = default;

		void analyse();
		void display();
		void save(IOFiles& w);
		void save();
		void edit();
		void summary();

	private:
		double bonus_test_;
		double bonus_coef_;
		double average_test_;
		double average_grades_with_bonus_;

		Vector<double> points_bonus_;
		Vector<double> grades_test_;
		Vector<double> grades_with_bonus_;
};
#endif
