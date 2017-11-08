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

		double bonus_points_;
		double bonus_coef_;
		Vector<unsigned int> questions_;
		double average_;
		double average_bonus_;
		double average_final_;
		unsigned int nfails_;

		Matrix<double> points_;
		Vector<double> test_bonus_;
		Vector<double> grades_;
		Vector<double> grades_bonus_;
		Vector<double> grades_final_;
		Vector<double> freq_final_;
};
#endif
