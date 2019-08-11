#ifndef DEF_DATE
#define DEF_DATE

#include "IOFiles.hpp"

class Date{
	public:
		/*!Default constructor*/
		Date() = default;
		/*!Default copy constructor*/
		Date(Date const&) = default;
		/*!Default move constructor*/
		Date(Date&&) = default;
		/*!Read constructor*/
		Date(IOFiles& r);
		/*!Default destructor*/
		~Date() = default;
		/*!Default assignment operator*/
		Date& operator=(Date const&) = default;

		void set(unsigned int const& d, unsigned int const& m, unsigned int const& y, bool const& silent = false);

		bool const& valid() const { return valid_; }
		unsigned int const& day() const { return day_; }
		unsigned int const& month() const { return month_; }
		unsigned int const& year() const { return year_; }

		std::string ddmmyyyy(char split='.') const;
		std::string yyyymmdd(char split='.') const;

		bool operator<(Date const& d) const;
		bool operator<=(Date const& d) const;
		bool operator>(Date const& d) const;
		bool operator>=(Date const& d) const;
		bool operator!=(Date const& d) const;
		bool operator==(Date const& d) const;

		std::string header_def() const { return ddmmyyyy('.'); }

	private:
		bool valid_ = true;
		unsigned int day_ = 1;
		unsigned int month_ = 1;
		unsigned int year_ = 1;
};

std::ostream& operator<<(std::ostream& flux, Date const& date);
std::istream& operator>>(std::istream& flux, Date& date);
IOFiles& operator<<(IOFiles& w, Date const& date);
IOFiles& operator>>(IOFiles& r, Date& date);
#endif
