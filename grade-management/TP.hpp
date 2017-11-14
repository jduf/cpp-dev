#ifndef DEF_TP
#define DEF_TP

#include "Note.hpp"
#include "ArrayOfStrings.hpp"

class TP: public Note{
	public:
		TP(unsigned int const& nstudents);
		TP(IOFiles& ftp);
		virtual ~TP() = default;
		/*{Forbidden*/
		TP() = delete;
		TP(TP const&) = delete;
		TP(TP&&) = delete;
		TP& operator=(TP);
		/*}*/

		void analyse();
		void display(VectorOfStrings const& class_list);
		void save(IOFiles& w);
		void add(unsigned int const& student);
		void edit(unsigned int const& student);
		void summary(Latex& latex, std::string const& class_id, VectorOfStrings const& class_list);
		void feedback(std::string const& class_id, VectorOfStrings const& class_list);

	private:
		VectorOfStrings criteria_;
		ArrayOfStrings comments_;
};
#endif
