#ifndef DEF_PARSEUR
#define DEF_PARSEUR

#include "Container.hpp"

class Parseur: public Container {
	public:
		/*{Description*/
		/*!Constructor
		 * \param argc=argc argument used in main(int argc, char* argv[])
		 * \param argv[]=argv[] argument used in main(int argc, char* argv[])
		 */
		/*}*/
		Parseur(unsigned int const& argc, char* argv[]);
		/*{Description*/
		/*!Constructor identical to Parseur(unsigned int const& argc, char* argv[]) but
		 * takes a list of flags that do not expect an arguments
		 */
		/*}*/
		Parseur(unsigned int const& argc, char* argv[], std::vector<std::string> const& flags);
		/*!Destructor that warns when an argument has not been set*/
		~Parseur();
		/*{Forbidden*/
		Parseur() = delete;
		Parseur(Parseur const&) = delete;
		Parseur(Parseur&&) = delete;
		Parseur& operator=(Parseur) = delete;
		/*}*/

		/*!If there is a var_[i]==pattern, returns val_[i] otherwise returns t*/
		template<typename Type>
			Type check_get(std::string const& pattern, Type const& t) const;
		/*!Returns true if there is a var_[i]==pattern and sets i*/
		bool find(std::string const& pattern, unsigned int& i, bool const& lock_iffail=false) const;
		/*!Returns true if there is a var_[i]==pattern*/
		bool find(std::string const& pattern) const;
		/*!The return value of find(*) decides whether to lock or not the class*/
		bool lock_if_not_found(bool const& has_status) const;

		/*!Returns locked_*/
		bool locked() const { return locked_; }
		/*!Returns type of argument (0=scalar,1=2=vector)*/
		unsigned int get_type(unsigned int const& i) const { return type_[i]; }

		void help();

	private:
		std::vector<unsigned int> type_;
		mutable std::vector<bool> used_;
		mutable bool locked_;
		mutable std::vector<std::string> searched_names_;

		template<typename Type>
			void set_vector_from_list(std::string const& name, std::string const& val);
		template<typename Type>
			void set_vector_from_range(std::string const& name, std::string const& val);
		void lock(std::string const& arg);
};

template<typename Type>
Type Parseur::check_get(std::string const& pattern, Type const& t) const {
	unsigned int i(0);
	if(find(pattern,i,false)){ return get<Type>(i); }
	else {
		std::cerr<<__PRETTY_FUNCTION__<<" : option set to default : '"<<pattern<<"="<<t<<"'"<<std::endl;
		return t;
	}
}

template<typename Type>
void Parseur::set_vector_from_list(std::string const& name, std::string const& val){
	std::vector<std::string> vitem(my::string_split(val,','));
	std::vector<Type> v(vitem.size());
	for(unsigned int i(0);i<vitem.size();i++){ v[i] = my::string2type<Type>(vitem[i]); }
	set(name.substr(3,name.size()-3),v);
}

template<typename Type>
void Parseur::set_vector_from_range(std::string const& name, std::string const& val){
	std::vector<std::string> vrange(my::string_split(val,':'));
	if(vrange.size() == 3){
		Type min(my::string2type<Type>(vrange[0]));
		Type dx (my::string2type<Type>(vrange[1]));
		Type max(my::string2type<Type>(vrange[2]));
		std::vector<Type> v((max-min)/dx+1);
		for(unsigned int j(0);j<v.size();j++){ v[j] = min+j*dx; }
		set(name.substr(3,name.size()-3),v);
	} else { std::cerr<<__PRETTY_FUNCTION__<<" : -t:name min:dx:max : "<<name<<std::endl; }
}
#endif
