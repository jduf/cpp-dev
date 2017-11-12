#ifndef DEF_EXAMEN
#define DEF_EXAMEN

#include "VectorOfStrings.hpp"
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
		std::string histogram(Vector<double> const& data, double const& min, double const& max, double const& bin_width, std::string const& title);
		void summary();
		void clean();

	private:
		std::string class_id_;
		std::string title_;
		VectorOfStrings class_list_;
		Matrix<double> points_;
		Vector<double> max_points_;
		Vector<double> grades_;
		double average_;
		unsigned int nfails_;

		double bonus_test_;
		double bonus_coef_;
		double average_test_;
		double average_grades_with_bonus_;

		Vector<double> points_bonus_;
		Vector<double> grades_test_;
		Vector<double> grades_with_bonus_;
};
#endif
