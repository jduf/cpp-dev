#ifndef DEF_CLASS
#define DEF_CLASS

#include "Latex.hpp"
#include "Examen.hpp"
#include "TP.hpp"

typedef std::pair<unsigned int,Note*> Evaluation;

class Class{
	public:
		Class();
		Class(IOFiles& r);
		virtual ~Class();
		/*{Forbidden*/
		Class(Class const&) = delete;
		Class(Class&&) = delete;
		Class& operator=(Class);
		/*}*/

		void edit(unsigned int e=-1);
		void add();
		void save();
		void summary();
		void clean();
		void display();

		unsigned int pick_student();

	private:
		std::string class_id_;
		VectorOfStrings class_list_;
		unsigned int nevals_;
		std::vector<Evaluation> evals_;
		Latex latex_;
};
#endif
