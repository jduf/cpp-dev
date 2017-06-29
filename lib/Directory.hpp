#ifndef DEF_DIRECTORY
#define DEF_DIRECTORY

#include "Miscellaneous.hpp"

#include <dirent.h>
#include <sys/stat.h>

class Directory{
	public:
		/*!Default constructor*/
		Directory() = default;
		/*!Default destructor*/
		~Directory() = default;
		/*!Removes all the path_+filename_+extension_ stored*/
		void set();
		/*{Forbidden*/
		Directory(Directory const&) = delete;
		Directory(Directory&&) = delete;
		Directory& operator=(Directory) = delete;
		/*}*/

		/*!Returns the ith filename_*/
		inline std::string const& get_name(unsigned int i) const { return filename_[i]; }
		/*!Returns the ith const& path_*/
		inline std::string const& get_path(unsigned int i) const { return path_[i]; }
		/*!Returns the ith const& extension_*/
		inline std::string const& get_ext(unsigned int i) const { return ext_[i]; }
		/*!Returns the ith const& path_+filename_+extension_*/
		inline std::string operator[](unsigned int i) const { return path_[i]+filename_[i]+ext_[i]; }
		/*!Returns the number of files*/
		inline unsigned int size() const { return path_.size(); }

		/*!Prints all the path_+filename_+extension_*/
		void print(std::ostream& flux) const;
		/*!Lists all directories stored in curr_dir*/
		void list_dir(std::string curr_dir);
		/*!Finds all files matching keyword*/
		void search_files(std::string const& keyword, std::string curr_dir, bool const& follow_link, bool const& recursive, bool const& hidden);
		/*!Finds all files with extension "ext"*/
		void search_files_ext(std::string const& ext, std::string curr_dir, bool const& follow_link, bool const& recursive, bool const& hidden);
		/*!Sorts by alphabetical order all the path_+filename_+extension_*/
		void sort();

	private:
		std::vector<std::string> path_;		//!< stores the paths
		std::vector<std::string> filename_;	//!< stores the filenames
		std::vector<std::string> ext_;		//!< stores the extensions

		/*!Splits a filename into filename_+ext_*/
		void split_ext(std::string f);
};
#endif
