#ifndef h5F38FB1E_2F10_4340_9D75_C0D6C18BB75C
#define h5F38FB1E_2F10_4340_9D75_C0D6C18BB75C

#include <boost/filesystem/path.hpp>
#include <vector>

std::vector< boost::filesystem::path > read_module_dependencies( boost::filesystem::path dependency_file_path );

#endif
