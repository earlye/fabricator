#include "build_target.hpp"

#include "failure.hpp"

#include <boost/process.hpp> 
#include <boost/process/mitigate.hpp>

#include <algorithm>
#include <iostream>

void build_target( fab::settings& settings )
{
  using namespace boost::process;
  using namespace boost::process::initializers;

  boost::filesystem::path command = boost::process::search_path(settings.compiler());

  std::vector<std::string> args;
  args.push_back(command.string());
  //std::copy(settings.cxxflags().begin(),settings.cxxflags().end(),std::back_inserter(args));
  //args.push_back("-v");
  //args.push_back("-MD");
  args.push_back("-o");
  args.push_back(settings.target());
  for( auto obj = settings.objects().begin(); obj != settings.objects().end() ; ++obj )
    {
      args.push_back(obj->string());
    }

  std::cout << "Linking " << settings.target() << "\n - ";  
  std::copy( args.begin(), args.end() , std::ostream_iterator<std::string>(std::cout, " "));
  std::cout << std::endl;

  child c = execute(set_args(args),inherit_env()); 
  auto exit_code = wait_for_exit(c);

  if (BOOST_PROCESS_EXITSTATUS(exit_code))
    {
      throw failure( exit_code, "Link failed" );
    }
  
}
