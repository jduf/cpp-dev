#ifndef DEF_TP
#define DEF_TP

#include "Note.hpp"
#include "ArrayOfStrings.hpp"

class TP: public Note{
	public:
		TP(Parseur& P);
		TP(IOFiles& ftp);
		TP() = delete;
		virtual ~TP() = default;

		void analyse();
		void display();
		void save(IOFiles& w);
		void save();
		void add();
		void edit(unsigned int student=-1);
		void summary();
		void feedback();
		void pdf(){
			summary();
			feedback();
		}

	private:
		VectorOfStrings criteria_;
		ArrayOfStrings comments_;
};
#endif
