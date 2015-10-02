#include "IOFiles.hpp"

/*constructors and destructor*/
/*{*/
IOFiles::IOFiles(std::string const& filename, bool const& write):
	filename_(filename),
	header_(NULL),
	write_(write),
	binary_(test_ext()),
	file_(filename_.c_str(), (binary_?(write_?(std::ios::out | std::ios::binary):(std::ios::in | std::ios::binary)) : (write_?std::ios::out : std::ios::in))),
	open_(file_.is_open())
{
	if(open_){
		if(binary_){
			if(write_){ header_->init(filename_); }
			else { read_header(); }
		}
	} else { std::cerr<<__PRETTY_FUNCTION__<<" : failed to open "<< filename_<<std::endl; }
}

IOFiles::~IOFiles(){
	if(open_ && binary_){
		if(write_){ write_header(); }
		delete header_;
	}
	file_.close();
}

std::string const IOFiles::endl("\n");
/*}*/

/*private methods used in the constructors, destructor or with open(std::string filename_)*/
/*{*/
bool IOFiles::test_ext(){
	std::string ext("bin");
	if(filename_.find(ext, (filename_.size() - ext.size())) != std::string::npos){
		ext = ".jd" + ext;
		if(filename_.find(ext, (filename_.size() - ext.size())) != std::string::npos){ header_ = new Header; }
		return true;
	} else { return false; }
}

void IOFiles::read_header(){
	unsigned int N(0);
	file_.seekg(-sizeof(unsigned int),std::ios::end);
	file_.read((char*)(&N),sizeof(unsigned int));
	file_.seekg(-sizeof(char)*N-sizeof(unsigned int),std::ios::end);
	std::string h;
	h.resize(N);
	file_.read(&h[0],N);
	header_->set(h);
	file_.seekg(0,std::ios::beg);
}

void IOFiles::write_header(){
	std::string t(header_->get());
	unsigned int N(t.size());
	file_.write(t.c_str(),N);
	file_.write((char*)(&N),sizeof(unsigned int));
}

void IOFiles::read_string(std::string& t){
	if(open_ && !write_){
		if (binary_){
			unsigned int N(0);
			file_.read((char*)(&N),sizeof(unsigned int));
			t.resize(N);
			file_.read(&t[0],N);
		} else {
			std::string tmp("");
			while(file_.good()){
				getline(file_,tmp);
				t += tmp + "\n";
			}
		}
	} else {
		std::cerr<<__PRETTY_FUNCTION__<<" : can't read from "<<filename_<<std::endl;
	}
}

void IOFiles::write_string(const char* t, unsigned int const& N){
	if(open_ && write_){
		if (binary_){
			file_.write((char*)(&N),sizeof(unsigned int));
			file_.write(t,N);
		} else { file_<<t<<std::flush; }
	} else {
		std::cerr<<__PRETTY_FUNCTION__<<" : can't write in "<<filename_<<std::endl;
	}
}
/*}*/

/*public methods*/
/*{*/
void IOFiles::precision(unsigned int const& N){
	if(binary_){ std::cerr<<__PRETTY_FUNCTION__<<" : has no effect on a binary file"; }
	else{ file_.precision(N); }
}

std::string IOFiles::get_header() const {
	if(header_ && open_){ return header_->get(); }
	std::cerr<<__PRETTY_FUNCTION__<<" : can't read from "<<filename_<<std::endl;
	return 0;
}
/*}*/
