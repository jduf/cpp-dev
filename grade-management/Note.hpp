#ifndef DEF_NOTE
#define DEF_NOTE

#include "VectorOfStrings.hpp"
#include "Gnuplot.hpp"
#include "Parseur.hpp"
#include "Latex.hpp"

class Note{
	public:
		Note(unsigned int const& nstudents);
		Note(IOFiles& fexa);
		virtual ~Note() = default;
		/*{Forbidden*/
		Note() = delete;
		Note(Note const&) = delete;
		Note(Note&&) = delete;
		Note& operator=(Note&);
		/*}*/

		virtual void edit(unsigned int const& student) = 0;
		virtual void analyse() = 0;
		virtual void display(VectorOfStrings const& class_list) = 0;
		virtual void summary(Latex& latex, std::string const& class_id, VectorOfStrings const& class_list) = 0;
		virtual void save(IOFiles& w);
		std::string histogram(Vector<double> const& data, double const& min, double const& max, double const& bin_width, std::string const& class_id, std::string const& title);

		std::string const& get_title() const { return title_; }

	protected:
		std::string title_;
		Matrix<double> points_;
		Vector<double> max_points_;
		Vector<double> grades_;
		double average_;
		unsigned int nfails_;
		unsigned int nvalid_;
};
#endif
