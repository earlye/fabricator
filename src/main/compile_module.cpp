#include "compile_module.hpp"

#include "failure.hpp"
#include "module_needs_build.hpp"
#include "replace_extension.hpp"

#include <boost/process.hpp> 
#include <boost/process/mitigate.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

boost::filesystem::path compile_module(fab::settings& settings, boost::filesystem::path const& path)
{
  using namespace boost::process;
  using namespace boost::process::initializers;

  boost::filesystem::path module_target = replace_extension(path, ".o");

  if ( module_needs_build(settings,path) )
    {
      std::vector<std::string> args;

      boost::filesystem::path command = boost::process::search_path(settings.compiler());
      
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
      if (BOOST_PROCESS_EXITSTATUS(exit_code))
	{
	  throw failure( BOOST_PROCESS_EXITSTATUS(exit_code), "Compile failed on module:" + path.string() );
	}
    }
  else
    {
      // std::cout << "Not re-compiling " << path << std::endl;
    }

  return module_target;
}
