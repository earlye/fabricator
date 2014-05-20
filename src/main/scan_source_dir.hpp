#ifndef h401435B5_C76A_4698_9D3C_22D1B70F8B08
#define h401435B5_C76A_4698_9D3C_22D1B70F8B08

#include "settings.hpp"

#include <boost/filesystem.hpp>

void scan_source_dir(fab::settings& settings, ::boost::filesystem::path path);
void scan_source_dir_entry(fab::settings& settings, ::boost::filesystem::directory_entry entry);

#endif
