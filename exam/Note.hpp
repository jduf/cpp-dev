#ifndef DEF_NOTE
#define DEF_NOTE

#include "VectorOfStrings.hpp"
#include "Gnuplot.hpp"
#include "Parseur.hpp"

class Note{
	public:
		Note(Parseur& P);
		Note(IOFiles& fexa);
		Note() = delete;
		virtual ~Note() = default;

		virtual void analyse() = 0;
		virtual void summary(std::string const& class_id, VectorOfStrings const& class_list) = 0;
		virtual void save(IOFiles& w);
		std::string histogram(Vector<double> const& data, double const& min, double const& max, double const& bin_width, std::string const& title);

		VectorOfStrings const& get_class_list() const { return class_list_; }

	protected:
		std::string class_id_;
		std::string title_;
		VectorOfStrings class_list_;
		Matrix<double> points_;
		Vector<double> max_points_;
		Vector<double> grades_;
		double average_;
		unsigned int nfails_;

		unsigned int pick_student();
};
#endif
