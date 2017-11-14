#include "Class.hpp"

Class::Class():
	class_id_(my::get_string("Classe :")),
	nevals_(0),
	latex_("./",class_id_)
{
	Linux command;
	std::string tmps;
	std::vector<std::string> tmp;
	unsigned int n;
	do {
		{
			command("vim " + class_id_ +"-list.txt",false);
			std::fstream file(class_id_+"-list.txt",std::ios::in);
			while(std::getline(file,tmps)){ tmp.push_back(tmps); }
		}
		n = tmp.size();
		class_list_.set(n);
		for(unsigned int i(0);i<n;i++){ class_list_(i) = tmp[i]; }
		tmp.clear();
		std::cout<<class_list_<<std::endl;
	} while ( !my::get_yn(my::tostring(class_list_.size())+" élèves enregistrés, est-ce correct?") );
	command("rm " + class_id_ +"-list.txt",false);
}

Class::Class(IOFiles& r):
	class_id_(r.read<std::string>()),
	class_list_(r.read<VectorOfStrings>()),
	nevals_(r.read<unsigned int>()),
	latex_("./",class_id_)
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

void Class::edit(unsigned int e){
	if(e>evals_.size()){
		for(unsigned int i(0);i<evals_.size();i++){
			std::cout<<i<<". "<<evals_[i].second->get_title()<<std::endl;
		}
		e = my::get_number("Choisir le test :", (unsigned int)(0), (unsigned int)(evals_.size()-1));
	}
	std::cout<<"Note sélectionnée : "<<evals_[e].second->get_title()<<std::endl;
	evals_[e].second->display(class_list_);
	evals_[e].second->edit(pick_student());
}

void Class::add(){
	switch(my::get_number("Choisir le type de note :",1,2)){
		case 1:{ evals_.push_back(Evaluation(1,new Examen(class_list_.size()))); }break;
		case 2:{ evals_.push_back(Evaluation(2,new TP(class_list_.size()))); }break;
	}
	for(unsigned int i(0);i<class_list_.size();i++){
		std::cout<<"Etudiant sélectionné : "<<class_list_(i)<<std::endl;
		evals_.back().second->edit(i);
	}
}

void Class::save(){
	IOFiles w("class/"+class_id_+"/"+class_id_+"-"+Time().date("-")+".jdbin",true,false);
	w.write("Classe",class_id_);
	w<<class_list_<<nevals_;
	for(auto const& e:evals_){
		w<<e.first;
		e.second->save(w);
	}
}

void Class::summary(){
	latex_.documentclass("article");
	latex_.package("geometry","a4paper,margin=1cm");
	latex_.package("babel","frenchb");
	latex_.package("fontenc","T1");
	latex_.package("inputenc","utf8");
	latex_.package("amsmath");
	latex_.package("siunitx");
	latex_.package("graphicx");
	latex_.package("xcolor","table");
	latex_ += "\\pagenumbering{gobble}";
	latex_.begin("document");
	for(auto const& e:evals_){
		e.second->summary(latex_,class_id_, class_list_);
		latex_ += "\\newpage";
	}
	latex_.end("document");
	latex_.save_file();
	latex_.pdflatex(true);
}

void Class::clean(){
	Linux command;
	command("rm histogram-* "+class_id_+".tex *.aux *.log",false);
}

unsigned int Class::pick_student(){
	unsigned int s(my::get_number("Choisir l'étudiant :", (unsigned int)(0), class_list_.size()-1));
	std::cout<<"Etudiant sélectionné : "<<class_list_(s)<<std::endl;
	return s;
}
