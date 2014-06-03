#ifndef hF7050551_9DAB_43DA_B7D8_65EE48AA81F8
#define hF7050551_9DAB_43DA_B7D8_65EE48AA81F8

#include "settings.hpp"

#include <boost/filesystem/path.hpp>

void build_target( fab::settings& settings );
void build_target( fab::settings& settings, boost::filesystem::path target, std::set< boost::filesystem::path > const& objects , boost::filesystem::path main_object );

#endif
