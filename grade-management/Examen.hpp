#ifndef DEF_EXAMEN
#define DEF_EXAMEN

#include "Note.hpp"

class Examen: public Note{
	public:
		Examen(unsigned int const& nstudents);
		Examen(IOFiles& fexa);
		~Examen() = default;
		/*{Forbidden*/
		Examen() = delete;
		Examen(Examen const&) = delete;
		Examen(Examen&&) = delete;
		Examen& operator=(Examen);
		/*}*/

		void analyse();
		void display(VectorOfStrings const& class_list);
		void save(IOFiles& w);
		void edit(unsigned int const& student);
		void summary(Latex& latex, std::string const& class_id, VectorOfStrings const& class_list);

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
