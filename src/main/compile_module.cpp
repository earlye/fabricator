#include "compile_module.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/process.hpp> 
#include <boost/process/mitigate.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

std::vector< boost::filesystem::path > read_dependencies( boost::filesystem::path dependency_file_path )
{
  boost::filesystem::ifstream dependency_file( dependency_file_path );
  std::vector<boost::filesystem::path> result;
  while ( !dependency_file.eof() )
    {
      std::string dependency_line;
      std::getline(dependency_file,dependency_line);

      std::string::size_type colon = dependency_line.find(':');
      if ( colon != std::string::npos )
	dependency_line = dependency_line.substr(colon+1);

      boost::algorithm::trim_right_if(dependency_line, boost::algorithm::is_any_of( "\\" ) );
      boost::algorithm::trim(dependency_line);

      std::vector< std::string > dependencies;
      boost::algorithm::split(dependencies,dependency_line,boost::algorithm::is_any_of(" \t"));
      for(auto d = dependencies.begin() ; d!= dependencies.end() ; ++d )
	{
	  if ( d->length() )
	    result.push_back(boost::filesystem::path(*d));
	}
    }
  return result;
}

boost::filesystem::path replace_extension( boost::filesystem::path source , boost::filesystem::path const& new_extension )
{
  source.replace_extension( new_extension );
  return source;
}

bool need_build(fab::settings& settings, boost::filesystem::path const& path )
{

  if ( settings.build_all() )
    return true;

  // If there isn't a dependency_file, then we want to build again, so we'll have one going forward.
  boost::filesystem::path dependency_file = replace_extension(path, ".d");
  if ( !boost::filesystem::exists(dependency_file) )
    {
      std::cout << dependency_file << " does not exist." << std::endl;
      return true;
    }

  boost::filesystem::path module_target = replace_extension(path, ".o");
  if ( !boost::filesystem::exists(module_target))
    {
      std::cout << module_target << " does not exist." << std::endl;
      return true;
    }

  // check timestamp vs timestamp on the dependency file.
  std::time_t source_time = boost::filesystem::last_write_time( path );
  std::time_t dependency_time = boost::filesystem::last_write_time( dependency_file );
  if ( source_time > dependency_time )
    {
      std::cout << path << " is newer than " << dependency_file << std::endl;
      return true;
    }

  // check timestamp vs timestamp on the object file
  std::time_t module_target_time = boost::filesystem::last_write_time( module_target );
  if ( source_time > module_target_time )
    {
      std::cout << path << " is newer than " << module_target << std::endl;
      return true;
    }

  // read the dependency file and check timestamps of all dependencies
  std::vector<boost::filesystem::path> dependencies = read_dependencies( dependency_file );
  
  for( auto dep = dependencies.begin() ; dep != dependencies.end() ; ++dep )
    {
      if (!boost::filesystem::exists(*dep) )
	{
	  std::cout << *dep << " does not exist" << std::endl;
	  return true;
	}


      dependency_time = boost::filesystem::last_write_time(*dep);
      if ( dependency_time > module_target_time )
	{
	  std::cout << *dep << " is newer than " << module_target << std::endl;
	  return true;
	}

    }

  return false;
}

void compile_module(fab::settings& settings, boost::filesystem::path const& path)
{
  using namespace boost::process;
  using namespace boost::process::initializers;

  boost::filesystem::path module_target = replace_extension(path, ".o");
  settings.objects_insert(module_target);

  if ( need_build(settings,path) )
    {
      std::vector<std::string> args;

      boost::filesystem::path command = boost::process::search_path("g++-4.9");
      
      args.push_back(command.string());
      std::copy(settings.cxxflags().begin(),settings.cxxflags().end(),std::back_inserter(args));
      //      args.push_back("-v");
      args.push_back("-MD");
      args.push_back("-c");
      args.push_back("-o");
      args.push_back(module_target.string());
      args.push_back(path.string());

      std::cout << "Compiling " << path << "\n - ";
      std::copy( args.begin(), args.end() , std::ostream_iterator<std::string>(std::cout, " "));
      std::cout << std::endl;
      
      child c = execute( set_args(args) );
      auto exit_code = wait_for_exit(c);
      std::cout << " - Exit Code:" << exit_code << std::endl;
      if (BOOST_PROCESS_EXITSTATUS(exit_code))
	{
	  throw std::runtime_error( "Compile failed on module:" + path.string() );
	}
    }
  else
    {
      std::cout << "Not re-compiling " << path << std::endl;
    }
}
