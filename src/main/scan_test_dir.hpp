#ifndef hB39ED747_9C0E_48C9_8B34_66EDE9092353
#define hB39ED747_9C0E_48C9_8B34_66EDE9092353

#include "settings.hpp"

#include <boost/filesystem.hpp>

void scan_test_dir(fab::settings& settings, ::boost::filesystem::path path);
void scan_test_dir_entry(fab::settings& settings, ::boost::filesystem::directory_entry entry);

#endif
