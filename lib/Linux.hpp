#ifndef DEF_LINUX
#define DEF_LINUX

#ifndef DEF_CONFIG
#pragma GCC warning "The macros defined in 'lib/config.mk' are essential, include this file in the makefile"
#endif

#include <cstdlib>
#include <string>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include "sys/stat.h"
#include "string.h"

class Linux{
	public:
		/*!Default constructor*/
		Linux() = default;
		/*!Default destructor*/
		~Linux() = default;
		/*{Forbidden*/
		Linux(Linux const&) = delete;
		Linux(Linux&&) = delete;
		Linux& operator=(Linux) = delete;
		/*}*/

		/*!Execute a UNIX command and get its exit value*/
		void operator()(std::string cmd, bool silent);
		static void open(std::string const& filename);
		static void close(bool const& run_now);

		/*!Returns exit value of the last command*/
		int status(){ return ev_; }
		/*!Returns a string containing the current path*/
		std::string pwd(){ 
			char bufA[216];
			char* answer = getcwd(bufA, sizeof(bufA));
			if(answer){ return std::string(answer)+"/"; }
			else {
				if(errno==ERANGE){ 
					std::cerr<<__PRETTY_FUNCTION__<<" : pathname too long, triyng longer... ";
					char bufB[512];
					answer = getcwd(bufB, sizeof(bufB));
					if(answer){ 
						std::cerr<<"success"<<std::endl; 
						return std::string(answer)+"/"; 
					} else { std::cerr<<"failure"<<std::endl; }
				} 
			}
			std::cerr<<__PRETTY_FUNCTION__<<": unhandled error"<<std::endl;
			return "/tmp/";
			/*The following doesn't work on MAC*/
			//return std::string(get_current_dir_name()) + '/'; 
		}

		/*!Creates a directory*/
		void mkdir(const char *directory, mode_t mode = 0755);
		/*!Creates a path*/
		void mkpath(const char *path, mode_t mode = 0755);

		static std::string latex    (std::string const& path, std::string const& filename);
		static std::string pdflatex (std::string const& path, std::string const& filename);
		static std::string rst2latex(std::string const& infile, std::string const& outfile);
		static std::string rst2html (std::string const& infile, std::string const& outfile);
		static std::string dvipdf   (std::string const& infile, std::string const& outfile);
		static std::string pdfcrop  (std::string const& infile, std::string const& outfile);
		static std::string pdf2png  (std::string const& infile, std::string const& outfile);
		static std::string pdf2jpg  (std::string const& infile, std::string const& outfile);
		/*{*//*!Creates gnuplot plots
			   Using a simple gnuplot file (with extension .gp) creates and
			   .eps picture and .tex file which can be used to create .pdf
			   files via Linux::pdflatex
			   A default size size is set such that its ratio equals the
			   golden number and when reduced by 70%, fits perfectly in one
			   column in revtex-4.1 articles
			   To specify a personal ratio, the first line of the .gp file
			   must start with "#latex_size x,y" where x,y are dimension of
			   the desired picture *//*}*/
		static std::string gp2latex(std::string const& inpath, std::string const& infile, std::string const& outpath, std::string const& outfile);
		static std::string html_browser(std::string const& html);

	private:
		class Bash{
			public:
				Bash() = default;
				~Bash();
				std::ofstream file_;
				std::string filename_;
				/*{Forbidden*/
				Bash(Bash const&) = delete;
				Bash(Bash&&) = delete;
				Bash& operator=(Bash) = delete;
				/*}*/
		};

		int ev_ = 0;//!< exit value of the last UNIX command
		static Bash bash_;
};
#endif
