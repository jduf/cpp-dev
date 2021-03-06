#ifndef DEF_ARRAYOFSTRINGS
#define DEF_ARRAYOFSTRINGS

#include "Vector.hpp"

class ArrayOfStrings{
	public:
		/*!Default constructor that initializes *m to NULL and N to 0*/
		ArrayOfStrings() = default;
		/*!Initializes a static array of std::string of size N_row*N_col*/
		ArrayOfStrings(unsigned int N_row, unsigned int N_col);
		/*!Initializes a static array of std::string of size N_row*N_col to a value val*/
		ArrayOfStrings(unsigned int N_row, unsigned int N_col, std::string val);
		/*!Deep copy constructor*/
		ArrayOfStrings(ArrayOfStrings const& aos);
		/*!Move constructor (can't be default because of arr_)*/
		ArrayOfStrings(ArrayOfStrings&& aos);
		/*!Constructor that reads from file*/
		ArrayOfStrings(IOFiles& r);
		/*!Delete the static array*/
		virtual ~ArrayOfStrings();

		/*!Accesses the (i,j)th entry of the array*/
		std::string const& operator()(unsigned int const& i, unsigned int const& j) const
		{ assert(i<row_ && j<col_); return arr_[i+j*row_]; }
		/*!Sets the (i,j)th entry of the array*/
		std::string& operator()(unsigned int const& i, unsigned int const& j)
		{ assert(i<row_ && j<col_); return arr_[i+j*row_]; }

		/*!Assignment (using Copy-And-Swap Idiom)*/
		ArrayOfStrings& operator=(ArrayOfStrings aos);

		/*!Set the array to 0*/
		void set();
		/*!Set the array to val*/
		void set(std::string const& val);
		/*!Set a N_row x N_col array  */
		void set(unsigned int N_row, unsigned int N_col);
		/*!Set a N_row x N_col array to val */
		void set(unsigned int N_row, unsigned int N_col, std::string val);

		/*!Returns the size of the array*/
		unsigned int const& size() const { return size_; }
		/*!Returns the number of rows of the array*/
		unsigned int const& row() const { return row_; }
		/*!Returns the number of columns of the array*/
		unsigned int const& col() const { return col_; }
		/*!Returns the pointer to the array*/
		std::string* ptr() const { return arr_; }

		std::string header_def() const { return "ArrayOfStrings("+RST::math(my::tostring(row_)+"\\times"+my::tostring(col_))+")"; }

	protected:
		unsigned int row_  = 0; //!< number of rows
		unsigned int col_  = 0; //!< number of columns
		unsigned int size_ = 0; //!< size of the array
		std::string* arr_ = NULL;//!< pointer to a static array

		/*!Copy-And-Swap Idiom*/
		void swap_to_assign(ArrayOfStrings& m1,ArrayOfStrings& m2);
};

std::ostream& operator<<(std::ostream& flux, ArrayOfStrings const& m);
std::istream& operator>>(std::istream& flux, ArrayOfStrings& m);
IOFiles& operator>>(IOFiles& r, ArrayOfStrings& m);
IOFiles& operator<<(IOFiles& w, ArrayOfStrings const& m);
#endif
