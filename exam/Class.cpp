#include "Class.hpp"

Class::Class(IOFiles& r):
	class_id_(r.read<std::string>()),
	class_list_(r.read<VectorOfStrings>()),
	nevals_(r.read<unsigned int>())
{
	for(unsigned int i(0);i<nevals_;i++){
		switch(r.read<unsigned int>()){
			case 1:{ evals_.push_back(Evaluation(1,new Examen(r))); }break;
			case 2:{ evals_.push_back(Evaluation(2,new TP(r))); }break;
		}
	}
}

Class::~Class(){
	for(auto& e:evals_){
		delete e.second;
		e.second = NULL;
	}
}

void Class::save(){
	IOFiles w(class_id_+"-"+Time().date("-")+".jdbin",true,false);
	w.write("Classe",class_id_);
	w<<class_list_<<nevals_;
	for(auto const& e:evals_){
		w<<e.first;
		e.second->save(w);
	}
}

void Class::init(std::string const& class_id, std::string const& exa, std::string const& tp){
	class_id_ = class_id;
	nevals_=2;
	IOFiles fexa(exa,false,false);
	evals_.push_back(Evaluation(1,new Examen(fexa)));
	IOFiles ftp(tp,false,false);
	if(ftp.is_open()){
		evals_.push_back(Evaluation(2,new TP(ftp)));
		class_list_ = evals_.back().second->get_class_list();
	}

}

void Class::summary(){
	for(auto const& e:evals_){
		e.second->summary();
	}
}
