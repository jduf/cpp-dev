#include "Parseur.hpp"

Parseur::Parseur(unsigned int const& argc, char* argv[]):
	locked_(false)
{
	std::string name;
	std::string val;
	for(unsigned int i(1);i<argc;i++){
		name = argv[i];
		type_.push_back(0);
		if(name[0] == '-'){
			used_.push_back(false);
			/*check if the type is specified*/
			if(name.find(":") != std::string::npos){
				if(i+1<argc){
					val = argv[i+1];
					if(name[1] == 's'){ set(name.substr(3),val); }
					else {
						if(val.find(":") != std::string::npos){ type_.back() = 1; }
						if(val.find(",") != std::string::npos){ type_.back() = 2; }
						switch(type_.back()){
							case 0:
								{
									switch(name[1]){
										case 'i':{ set(name.substr(3),my::string2type<int>(val)); } break;
										case 'u':{ set(name.substr(3),my::string2type<unsigned int>(val)); } break;
										case 'd':{ set(name.substr(3),my::string2type<double>(val)); } break;
										case 'b':{ set(name.substr(3),my::string2type<bool>(val)); } break;
										default: { lock(name); }
									}
								}break;
							case 1:
								{
									switch(name[1]){
										case 'i':{ set_vector_from_range<int>(name,val); } break;
										case 'u':{ set_vector_from_range<unsigned int>(name,val); } break;
										case 'd':{ set_vector_from_range<double>(name,val); } break;
										default: { lock(name); }
									}
								}break;
							case 2:
								{
									switch(name[1]){
										case 'i':{ set_vector_from_list<int>(name,val); } break;
										case 'u':{ set_vector_from_list<unsigned int>(name,val); } break;
										case 'd':{ set_vector_from_list<double>(name,val); } break;
										default: { lock(name); }
									}
								}break;
						}
					}
					i++;
				} else {
					val = "NULL";
					set(name.substr(3),val);
				}
			} else {
				/*!handles linux like options (e.g.  'ls -l' or 'head -n 5')*/
				if(i+1<argc){
					/*!if there are other argument in the list, check the
					 * nature of the next one*/
					std::string next_arg(argv[i+1]);
					if(next_arg[0] == '-'){
						/*!if the next argument has a '-'*/
						if(next_arg.find(":") != std::string::npos){
							/*!if the next argument has also a ':' the current
							 * argument can be accepted as a linux like
							 * argument*/
							set(name.substr(1),name.substr(1));
						} else {
							/*!this is problematic because there is no way to
							 * know if it is a value related to the current
							 * argument or a new argument*/
							std::cerr<<__PRETTY_FUNCTION__<<" : problematic argument (impossible to know if '"
								<<next_arg<<"' is an option related to '"<<name<<"' or a new argument)"<<std::endl;
							locked_ = true;
						}
					} else {
						/*!in that case the next argument is considered to be
						 * an option related to the current argument*/
						set(name.substr(1),next_arg);
						i++;
					}
				} else {
					val = "NULL";
					set(name.substr(1),val);
				}
			}
		} else {
			set(my::tostring(i),name);
			used_.push_back(true);
		}
	}
}

Parseur::Parseur(unsigned int const& argc, char* argv[], std::vector<std::string> const& flags):
	locked_(false)
{
	std::string name;
	std::string val;
	for(unsigned int i(1);i<argc;i++){
		name = argv[i];
		type_.push_back(0);
		if(name[0] == '-'){
			used_.push_back(false);
			/*check if the type is specified*/
			if(name.find(":") != std::string::npos){
				if(i+1<argc){
					val = argv[i+1];
					if(name[1] == 's'){ set(name.substr(3),val); }
					else {
						if(val.find(":") != std::string::npos){ type_.back() = 1; }
						if(val.find(",") != std::string::npos){ type_.back() = 2; }
						switch(type_.back()){
							case 0:
								{
									switch(name[1]){
										case 'i':{ set(name.substr(3),my::string2type<int>(val)); } break;
										case 'u':{ set(name.substr(3),my::string2type<unsigned int>(val)); } break;
										case 'd':{ set(name.substr(3),my::string2type<double>(val)); } break;
										case 'b':{ set(name.substr(3),my::string2type<bool>(val)); } break;
										default: { lock(name); }
									}
								}break;
							case 1:
								{
									switch(name[1]){
										case 'i':{ set_vector_from_range<int>(name,val); } break;
										case 'u':{ set_vector_from_range<unsigned int>(name,val); } break;
										case 'd':{ set_vector_from_range<double>(name,val); } break;
										default: { lock(name); }
									}
								}break;
							case 2:
								{
									switch(name[1]){
										case 'i':{ set_vector_from_list<int>(name,val); } break;
										case 'u':{ set_vector_from_list<unsigned int>(name,val); } break;
										case 'd':{ set_vector_from_list<double>(name,val); } break;
										default: { lock(name); }
									}
								}break;
						}
					}
					i++;
				} else {
					val = "NULL";
					set(name.substr(3),val);
				}
			} else {
				/*!handles linux like options (e.g.  'ls -l' or 'head -n 5')*/
				bool is_flag(false);
				name = name.substr(1);
				for(unsigned int j(0);j<flags.size();j++){
					if(name == flags[j]){ 
						j = flags.size();
						is_flag = true;
					}
				}
				if(is_flag){
					set(name,name);
				} else {
					i++;
					if(i<argc){
						std::string val(argv[i]);
						if(val[0] == '-'){
							std::cerr<<__PRETTY_FUNCTION__<<" : the value of option '-"<<name<<"' starts with an '-' :"<<val<<std::endl; 
							lock(name);
						}
						set(name,val); 
					}
					else { std::cerr<<__PRETTY_FUNCTION__<<" : there is no argument to option '-"<<name<<"'"<<std::endl; }
				}
			}
		} else {
			set(my::tostring(i),name);
			used_.push_back(true);
		}
	}
}

Parseur::~Parseur(){
	for(unsigned int i(0);i<data_.size();i++){
		if(!used_[i]){ std::cerr<<__PRETTY_FUNCTION__<<" : variable "<<data_[i]->name_<<" was given as input but not used"<<std::endl; }
	}
	if(locked_){ std::cerr<<__PRETTY_FUNCTION__<<" : the parseur was locked"<<std::endl; }
}

bool Parseur::find(std::string const& pattern, unsigned int& i, bool const& lock_iffail) const {
	if(!locked_){
		if(Container::find(pattern,i,lock_iffail)){
			used_[i] = true;
			return true;
		} else {
			locked_ = lock_iffail;
			if(locked_){ std::cerr<<__PRETTY_FUNCTION__<<" : can't find "<<pattern<<", the parseur is locked"<<std::endl; }
			return false;
		}
	} else { return false; }
}

bool Parseur::find(std::string const& pattern) const {
	unsigned int i(0);
	return find(pattern,i);
}

void Parseur::lock(std::string const& arg){
	locked_ = true;
	std::cerr<<__PRETTY_FUNCTION__<<" : wrong argument '"<<arg<<"' : should be '-[iudsb]:name' : "<<std::endl;
}
