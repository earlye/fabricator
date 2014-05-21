#include "module_needs_build.hpp"

#include "read_module_dependencies.hpp"
#include "replace_extension.hpp"

bool module_needs_build(fab::settings& settings, boost::filesystem::path const& path )
{

  if ( settings.build_all() )
    {
      std::cout << path << " needs build: settings.build_all() is true" << std::endl;
      return true;
    }

  // If there isn't a dependency_file, then we want to build again, so we'll have one going forward.
  boost::filesystem::path dependency_file = replace_extension(path, ".d");
  if ( !boost::filesystem::exists(dependency_file) )
    {
      std::cout << path << " needs build: " << dependency_file << " does not exist." << std::endl;
      return true;
    }

  // If the target has not been build, then we want to build it, obviously.
  boost::filesystem::path module_target = replace_extension(path, ".o");
  if ( !boost::filesystem::exists(module_target))
    {
      std::cout << path << " needs build: " << module_target << " does not exist." << std::endl;
      return true;
    }

  // check timestamp vs timestamp on the dependency file.
  std::time_t source_time = boost::filesystem::last_write_time( path );
  std::time_t dependency_time = boost::filesystem::last_write_time( dependency_file );
  if ( source_time > dependency_time )
    {
      std::cout << path << " needs build: it is newer than " << dependency_file << std::endl;
      return true;
    }

  // check timestamp vs timestamp on the object file
  std::time_t module_target_time = boost::filesystem::last_write_time( module_target );
  if ( source_time > module_target_time )
    {
      std::cout << path << " needs build: it is newer than " << module_target << std::endl;
      return true;
    }

  // read the dependency file and check timestamps of all dependencies
  std::vector<boost::filesystem::path> dependencies = read_module_dependencies( dependency_file );
  
  for( auto dep = dependencies.begin() ; dep != dependencies.end() ; ++dep )
    {
      if (!boost::filesystem::exists(*dep) )
	{
	  std::cout << path << " needs build: " << *dep << " does not exist" << std::endl;
	  return true;
	}


      dependency_time = boost::filesystem::last_write_time(*dep);
      if ( dependency_time > module_target_time )
	{
	  std::cout << path << " needs build: " << *dep << " is newer than " << module_target << std::endl;
	  return true;
	}

    }

  //std::cout << path << " does not need build" << std::endl;
  return false;
}
