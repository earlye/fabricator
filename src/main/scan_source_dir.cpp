#include "scan_source_dir.hpp"

#include <boost/bind.hpp>

void scan_source_dir_entry(fab::settings& settings, boost::filesystem::directory_entry entry)
{
  boost::filesystem::path extension = entry.path().extension();
  if (!(extension == ".C" || extension == ".cpp" || extension == ".cxx" || extension == ".cc"))
    return;
  settings.source_modules_insert(entry.path());
}

void scan_source_dir(fab::settings& settings, boost::filesystem::path dir)
{
  using boost::filesystem::recursive_directory_iterator;
  std::for_each(recursive_directory_iterator(dir), recursive_directory_iterator(),boost::bind(scan_source_dir_entry,boost::ref(settings),_1));
}
