#ifndef DEF_CLASS
#define DEF_CLASS

#include "Examen.hpp"
#include "TP.hpp"

typedef std::pair<unsigned int,Note*> Evaluation;

class Class{
	public:
		Class() = default;
		Class(IOFiles& r);
		virtual ~Class();

		void init(std::string const& class_id, std::string const& exa, std::string const& tp);

		void save();
		void summary();

	private:
		std::string class_id_;
		VectorOfStrings class_list_;
		unsigned int nevals_;
		std::vector<Evaluation> evals_;
};
#endif
