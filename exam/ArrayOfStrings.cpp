#include "ArrayOfStrings.hpp"

/*constructors and destructor*/
/*{*/
ArrayOfStrings::ArrayOfStrings(unsigned int N_row, unsigned int N_col):
	row_(N_row),
	col_(N_col),
	size_(N_col*N_row),
	arr_(size_?new std::string[size_]:NULL)
{}

ArrayOfStrings::ArrayOfStrings(unsigned int N_row, unsigned int N_col, std::string val):
	row_(N_row),
	col_(N_col),
	size_(N_col*N_row),
	arr_(size_?new std::string[size_]:NULL)
{
	for(unsigned int i(0);i<size_;i++){ arr_[i] = val; }
}

ArrayOfStrings::ArrayOfStrings(ArrayOfStrings const& aos):
	row_(aos.row_),
	col_(aos.col_),
	size_(aos.size_),
	arr_(size_?new std::string[size_]:NULL)
{
	for(unsigned int i(0);i<size_;i++){ arr_[i] = aos.arr_[i]; }
}

ArrayOfStrings::ArrayOfStrings(ArrayOfStrings&& aos):
	row_(aos.row_),
	col_(aos.col_),
	size_(aos.size_),
	arr_(aos.arr_)
{
	aos.arr_ = NULL;
	aos.row_ = aos.col_ = aos.size_ = 0;
}

ArrayOfStrings::ArrayOfStrings(IOFiles& r):
	row_(r.read<unsigned int>()),
	col_(r.read<unsigned int>()),
	size_(row_*col_),
	arr_(size_?new std::string[size_]:NULL)
{ for(unsigned int i(0);i<size_;i++){ r>>arr_[i]; } }

ArrayOfStrings::~ArrayOfStrings(){
	if(arr_){
		delete[]  arr_;
		arr_ = NULL;
	}
}

void ArrayOfStrings::swap_to_assign(ArrayOfStrings& m1,ArrayOfStrings& m2){
	std::swap(m1.arr_,m2.arr_);
	std::swap(m1.row_,m2.row_);
	std::swap(m1.col_,m2.col_);
	std::swap(m1.size_,m2.size_);
}

ArrayOfStrings& ArrayOfStrings::operator=(ArrayOfStrings aos){
	swap_to_assign(*this,aos);
	return (*this);
}
/*}*/

/*i/o methods*/
/*{*/
std::ostream& operator<<(std::ostream& flux, ArrayOfStrings const& aos){
	for(unsigned int i(0);i<aos.row();i++){
		for(unsigned int j(0);j<aos.col();j++){ flux<<aos(i,j)<<" "; }
		if(i+1 != aos.row()){ flux<<std::endl; }
	}
	return flux;
}

std::istream& operator>>(std::istream& flux, ArrayOfStrings& aos){
	unsigned int size(aos.size());
	for(unsigned int i(0);i<size;i++){ flux>>aos.ptr()[i]; }
	return flux;
}

IOFiles& operator<<(IOFiles& w, ArrayOfStrings const& aos){
	if(w.is_binary()){
		w<<aos.row()<<aos.col();
		unsigned int size(aos.size());
		for(unsigned int i(0);i<size;i++){ w<<aos.ptr()[i]; }
	} else { w.stream()<<aos; }
	return w;
}

IOFiles& operator>>(IOFiles& r, ArrayOfStrings& aos){
	if(r.is_binary()){ aos = std::move(ArrayOfStrings(r)); }
	else { r.stream()>>aos; }
	return r;
}
/*}*/

/*methods that modify the class*/
/*{*/
void ArrayOfStrings::set(){
	if(arr_){ delete[] arr_; }
	arr_ = NULL;
	row_ = 0;
	col_ = 0;
	size_ = 0;
}

void ArrayOfStrings::set(std::string const& val){
	for(unsigned int i(0); i<size_; i++){ arr_[i] = val; }
}

void ArrayOfStrings::set(unsigned int row, unsigned int col){
	if(col_ != col || row_ != row){
		if(arr_){ delete[] arr_; }
		arr_ = new std::string[row*col];
		row_ = row;
		col_ = col;
		size_ = row*col;
	}
}

void ArrayOfStrings::set(unsigned int row, unsigned int col, std::string val){
	this->set(row,col);
	this->set(val);
}
/*}*/
