#include "read_module_dependencies.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/fstream.hpp>

#include <iostream>
#include <fstream>

std::vector< boost::filesystem::path > read_module_dependencies( boost::filesystem::path dependency_file_path )
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

