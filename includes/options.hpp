#ifndef __MODULE_OPTIONS__
#define __MODULE_OPTIONS__

#include <string>
#include <boost/program_options.hpp> // exceptions also

#include <filters.hpp>


class options
 {
  public:

      bool verbose;
      bool help;
      bool version;

      std::string input;
      std::string mask;
      std::string output;

      filter_type filter;
      bool magic;

      static void show_help();
      static void show_version();


  options()
   : verbose(false),
     help(false),
     version(false),
     filter(filter_type::X),
     magic(false)
  {}

  options(int, const char * const[]);

  ~options()=default;
 };


#endif
// __MODULE_OPTIONS__
