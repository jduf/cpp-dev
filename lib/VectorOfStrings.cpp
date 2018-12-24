#include "VectorOfStrings.hpp"

/*constructors and destructor*/
/*{*/
VectorOfStrings::VectorOfStrings(unsigned int N):
	size_(N),
	vos_(size_?new std::string[size_]:NULL)
{}

VectorOfStrings::VectorOfStrings(unsigned int N, std::string val):
	size_(N),
	vos_(size_?new std::string[size_]:NULL)
{
	for(unsigned int i(0);i<size_;i++){ vos_[i] = val; }
}

VectorOfStrings::VectorOfStrings(VectorOfStrings const& vos):
	size_(vos.size_),
	vos_(size_?new std::string[size_]:NULL)
{
	for(unsigned int i(0);i<size_;i++){ vos_[i] = vos.vos_[i]; }
}

VectorOfStrings::VectorOfStrings(VectorOfStrings&& vos):
	size_(vos.size_),
	vos_(vos.vos_)
{
	vos.vos_ = NULL;
	vos.size_ = 0;
}

VectorOfStrings::VectorOfStrings(IOFiles& r):
	size_(r.read<unsigned int>()),
	vos_(size_?new std::string[size_]:NULL)
{
	for(unsigned int i(0);i<size_;i++){ r>>vos_[i]; }
}

VectorOfStrings::~VectorOfStrings(){
	if(vos_){
		delete[]  vos_;
		vos_ = NULL;
	}
}

void VectorOfStrings::swap_to_assign(VectorOfStrings& vos1,VectorOfStrings& vos2){
	std::swap(vos1.vos_,vos2.vos_);
	std::swap(vos1.size_,vos2.size_);
}

VectorOfStrings& VectorOfStrings::operator=(VectorOfStrings vos){
	swap_to_assign(*this,vos);
	return (*this);
}
/*}*/

/*i/o methods*/
/*{*/
std::ostream& operator<<(std::ostream& flux, VectorOfStrings const& vos){
	unsigned int size(vos.size());
	for(unsigned int i(0);i<size;i++){
		flux<<vos.ptr()[i];
		if(i+1 != vos.size()){ flux<<std::endl; }
	}
	return flux;
}

std::istream& operator>>(std::istream& flux, VectorOfStrings& vos){
	unsigned int size(vos.size());
	for(unsigned int i(0);i<size;i++){ flux>>vos.ptr()[i]; }
	return flux;
}

IOFiles& operator<<(IOFiles& w, VectorOfStrings const& vos){
	if(w.is_binary()){
		unsigned int size(vos.size());
		w<<size;
		for(unsigned int i(0);i<size;i++){ w<<vos.ptr()[i]; }
	} else { w.stream()<<vos; }
	return w;
}

IOFiles& operator>>(IOFiles& r, VectorOfStrings& vos){
	if(r.is_binary()){ vos = std::move(VectorOfStrings(r)); }
	else { r.stream()>>vos; }
	return r;
}
/*}*/

/*methods that modify the class*/
/*{*/
void VectorOfStrings::set(){
	if(vos_){ delete[] vos_; }
	vos_ = NULL;
	size_ = 0;
}

void VectorOfStrings::set(std::string const& val){
	for(unsigned int i(0); i<size_; i++){ vos_[i] = val; }
}

void VectorOfStrings::set(unsigned int N){
	if(size_ != N){
		if(vos_){ delete[] vos_; }
		vos_ = new std::string[N];
		size_ = N;
	}
}

void VectorOfStrings::set(unsigned int N, std::string val){
	this->set(N);
	this->set(val);
}
/*}*/
