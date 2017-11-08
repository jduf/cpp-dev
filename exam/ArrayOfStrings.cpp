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

ArrayOfStrings::ArrayOfStrings(ArrayOfStrings const& mat):
	row_(mat.row_),
	col_(mat.col_),
	size_(mat.size_),
	arr_(size_?new std::string[size_]:NULL)
{
	for(unsigned int i(0);i<size_;i++){ arr_[i] = mat.arr_[i]; }
}

ArrayOfStrings::ArrayOfStrings(ArrayOfStrings&& mat):
	row_(mat.row_),
	col_(mat.col_),
	size_(mat.size_),
	arr_(mat.arr_)
{
	mat.arr_ = NULL;
	mat.row_ = mat.col_ = mat.size_ = 0;
}

ArrayOfStrings::ArrayOfStrings(IOFiles& r):
	row_(r.read<unsigned int>()),
	col_(r.read<unsigned int>()),
	size_(row_*col_),
	arr_(size_?new std::string[size_]:NULL)
{
	for(unsigned int i(0);i<size_;i++){ r>>arr_[i]; }
}

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

ArrayOfStrings& ArrayOfStrings::operator=(ArrayOfStrings mat){
	swap_to_assign(*this,mat);
	return (*this);
}
/*}*/

/*i/o methods*/
/*{*/
std::ostream& operator<<(std::ostream& flux, ArrayOfStrings const& m){
	for(unsigned int i(0);i<m.row();i++){
		for(unsigned int j(0);j<m.col();j++){ flux<<m(i,j)<<" "; }
		if(i+1 != m.row()){ flux<<std::endl; }
	}
	return flux;
}

std::istream& operator>>(std::istream& flux, ArrayOfStrings& m){
	unsigned int size(m.size());
	for(unsigned int i(0);i<size;i++){ flux>>m.ptr()[i]; }
	return flux;
}

IOFiles& operator<<(IOFiles& w, ArrayOfStrings const& m){
	if(w.is_binary()){
		w<<m.row()<<m.col();
		unsigned int size(m.size());
		for(unsigned int i(0);i<size;i++){ w<<m.ptr()[i]; }
	} else { w.stream()<<m; }
	return w;
}

IOFiles& operator>>(IOFiles& r, ArrayOfStrings& m){
	if(r.is_binary()){ m = std::move(ArrayOfStrings(r)); }
	else { r.stream()>>m; }
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
