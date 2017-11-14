#ifndef DEF_VECTOROFSTRINGS
#define DEF_VECTOROFSTRINGS

#include "Vector.hpp"

class VectorOfStrings{
	public:
		/*!Default constructor that initializes *m to NULL and N to 0*/
		VectorOfStrings() = default;
		/*!Initializes a static array of std::string of size N*/
		VectorOfStrings(unsigned int N);
		/*!Initializes a static array of std::string of size N to a value val*/
		VectorOfStrings(unsigned int N, std::string val);
		/*!Deep copy constructor*/
		VectorOfStrings(VectorOfStrings const& vos);
		/*!Move constructor (can't be default because of vos_)*/
		VectorOfStrings(VectorOfStrings&& vos);
		/*!Constructor that reads from file*/
		VectorOfStrings(IOFiles& r);
		/*!Delete the static array*/
		virtual ~VectorOfStrings();

		/*!Accesses the (i,j)th entry of the vector*/
		std::string const& operator()(unsigned int const& i) const
		{ assert(i<size_); return vos_[i]; }
		/*!Sets the (i,j)th entry of the vector*/
		std::string& operator()(unsigned int const& i)
		{ assert(i<size_); return vos_[i]; }

		/*!Assignment (using Copy-And-Swap Idiom)*/
		VectorOfStrings& operator=(VectorOfStrings vos);

		/*!Set the vector to 0*/
		void set();
		/*!Set the vector to val*/
		void set(std::string const& val);
		/*!Set a N vector */
		void set(unsigned int N);
		/*!Set a N vector to val */
		void set(unsigned int N, std::string val);

		/*!Returns the size of the vector*/
		unsigned int const& size() const { return size_; }
		/*!Returns the pointer to the vector*/
		std::string* ptr() const { return vos_; }

		std::string header_def() const { return "VectorOfStrings("+RST::math(my::tostring(size_))+")"; }

	protected:
		unsigned int size_ = 0; //!< size of the array
		std::string* vos_ = NULL;//!< pointer to a static array

		/*!Copy-And-Swap Idiom*/
		void swap_to_assign(VectorOfStrings& vos1,VectorOfStrings& vos2);
};

std::ostream& operator<<(std::ostream& flux, VectorOfStrings const& vos);
std::istream& operator>>(std::istream& flux, VectorOfStrings& vos);
IOFiles& operator>>(IOFiles& r, VectorOfStrings& vos);
IOFiles& operator<<(IOFiles& w, VectorOfStrings const& vos);
#endif
