#include "scan_child.hpp"

#include "contains.hpp"
#include "failure.hpp"

#include <boost/filesystem.hpp>
#include <boost/process.hpp> 
#include <boost/process/mitigate.hpp>

#include <vector>

void scan_child(fab::settings const& settings, boost::filesystem::directory_entry entry)
{
  using namespace boost::process;
  using namespace boost::process::initializers;

  boost::filesystem::path path = entry.path();
  if (!boost::filesystem::is_directory(entry))
    {
      return;
    }
  if (contains(settings.ignore(),path.filename()))
    {
      //std::cout << "ignoring " << path << std::endl;
      return;
    }

  std::vector<std::string> args;

  boost::filesystem::path command = settings.fab();
  if (!command.is_absolute() )
    command = boost::process::search_path(command.string());
  args.push_back(command.string());

  //std::cout << "Building " << path << std::endl;
  //std::cout << " - fab: " << settings.fab() << std::endl;
  //std::cout << " - command: ";
  //std::copy( args.begin(), args.end() , std::ostream_iterator<std::string>(std::cout, " "));
  //std::cout << std::endl;
  //std::cout << " - cwd:" << boost::filesystem::current_path() << std::endl;
  
  child c = execute
    ( 
     set_args(args), 
     start_in_dir(entry.path().string()), 
     inherit_env() 
      );
  //child c = execute(run_exe(command), start_in_dir(entry.path().string()));
  auto exit_code = wait_for_exit(c);
  if (BOOST_PROCESS_EXITSTATUS(exit_code))
    {
      throw failure( exit_code, "Recursive build failed on module:" + path.string() );
    }
}
