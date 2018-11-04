#ifndef DEF_DATE
#define DEF_DATE

#include "IOFiles.hpp"

class Date{
	public:
		/*!Default constructor*/
		Date() = default;
		/*!Read constructor*/
		Date(IOFiles& r);
		/*!Default destructor*/
		~Date() = default;

		void set(unsigned int d, unsigned int m, unsigned int y);

		bool const& valid() const { return valid_; }
		unsigned int const& day() const { return day_; }
		unsigned int const& month() const { return month_; }
		unsigned int const& year() const { return year_; }

		std::string ddmmyyyy(char split='.') const;

		bool operator<(Date const& d) const;
		bool operator<=(Date const& d) const;
		bool operator>(Date const& d) const;
		bool operator>=(Date const& d) const;
		bool operator!=(Date const& d) const;
		bool operator==(Date const& d) const;

	private:
		bool valid_ = true;
		unsigned int day_ = 1;
		unsigned int month_ = 1;
		unsigned int year_ = 2000;
};

std::ostream& operator<<(std::ostream& flux, Date const& date);
std::istream& operator>>(std::istream& flux, Date& date);
IOFiles& operator<<(IOFiles& w, Date const& date);
IOFiles& operator>>(IOFiles& r, Date& date);
#endif
