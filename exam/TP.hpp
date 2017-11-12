#ifndef DEF_TP
#define DEF_TP

#include "ArrayOfStrings.hpp"
#include "VectorOfStrings.hpp"
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
		std::string title_;
		VectorOfStrings class_list_;
		Matrix<double> points_;
		Vector<double> max_points_;
		Vector<double> grades_;
		double average_;
		unsigned int nfails_;

		VectorOfStrings criteria_;
		ArrayOfStrings comments_;

		unsigned int pick_student();
};
#endif
