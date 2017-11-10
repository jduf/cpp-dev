#ifndef DEF_EXAMEN
#define DEF_EXAMEN

#include "Vector.hpp"
#include "Gnuplot.hpp"
#include "Parseur.hpp"

class Examen{
	public:
		Examen(Parseur& P);
		Examen(IOFiles& fexa);
		Examen() = delete;
		~Examen() = default;

		void analyse();
		void display();
		void save();
		void edit();
		void histogram();
		void summary();
		void clean();

	private:
		std::string class_id_;
		std::string date_;
		std::vector<std::string> class_list_;

		double bonus_test_;
		double bonus_coef_;
		double average_;
		double average_grades_with_bonus_;
		double average_final_;
		unsigned int nfails_;

		Matrix<double> points_;
		Vector<double> max_points_;
		Vector<double> points_bonus_;
		Vector<double> grades_;
		Vector<double> grades_with_bonus_;
		Vector<double> grades_final_;
};
#endif
