#include "scan_child.hpp"

#include "contains.hpp"

void scan_child(fab::settings const& settings, boost::filesystem::directory_entry entry)
{
  boost::filesystem::path filename = entry.path().filename();
  if (!boost::filesystem::is_directory(entry))
    {
      return;
    }
  if (contains(settings.ignore(),(filename)))
    {
      std::cout << "ignoring " << filename << std::endl;
      return;
    }
  std::cout << entry.path().filename() << std::endl;
}
