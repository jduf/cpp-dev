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
		void display(VectorOfStrings const& class_list);
		void save(IOFiles& w);
		void add();
		void edit(unsigned int student=-1);
		void summary(Latex& latex, std::string const& class_id, VectorOfStrings const& class_list);
		void feedback(std::string const& class_id, VectorOfStrings const& class_list);

	private:
		VectorOfStrings criteria_;
		ArrayOfStrings comments_;
};
#endif
