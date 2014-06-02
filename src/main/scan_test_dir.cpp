#include "scan_test_dir.hpp"

#include "compile_module.hpp"

#include <boost/bind.hpp>

void scan_test_dir_entry(fab::settings& settings, boost::filesystem::directory_entry entry)
{
  boost::filesystem::path extension = entry.path().extension();
  if (!(extension == ".C" || extension == ".cpp" || extension == ".cxx" || extension == ".cc"))
    return;

  std::cout << "- test module:" << entry.path() << std::endl;
  compile_module(settings,entry);
}

void scan_test_dir(fab::settings& settings, boost::filesystem::path dir)
{
  using boost::filesystem::recursive_directory_iterator;
  for( auto iter = recursive_directory_iterator(dir) ; iter != recursive_directory_iterator() ; ++iter )
    {
      scan_test_dir_entry(settings, *iter);
    }
}

