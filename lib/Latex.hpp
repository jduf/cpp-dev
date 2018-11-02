#ifndef DEF_LATEX
#define DEF_LATEX

#include "IOFiles.hpp"
#include "Linux.hpp"

class Latex{
	public:
		Latex(std::string const& path, std::string const& filename, std::string const& documentclass, std::string const& options="");
		Latex(Latex&&) = default;
		~Latex() = default;
		/*{Forbidden*/
		Latex() = delete;
		Latex(Latex const&) = delete;
		Latex& operator=(Latex);
		/*}*/

		void package(std::string const& s, std::string const& options="");
		void begin(std::string const& s, std::string const& options="");
		void end(std::string const& s);
		void command(std::string const& s);
		void item(std::string const& s);

		void histogram(std::vector<double> const& data, unsigned int const& nbins, double const& min, double const& max, std::string const& options="");

		void operator=(std::string const& s);
		void operator+=(std::string const& s);
		
		void save_file();
		void pdflatex(bool const& silent, bool const& clean);

	protected:
		std::string path_;		//!< path of the .gp, .png and .pdf files
		std::string filename_;	//!< filename (without the extension)
		std::string latex_;		//!< text of the .tex file
};
#endif
