#include "object_list_functions.hpp"

#include "settings.hpp"

#include <boost/iostreams/stream.hpp>
#include <boost/process.hpp> 
#include <boost/process/mitigate.hpp>

std::vector<std::string> object_list_functions( fab::settings& settings, boost::filesystem::path object ) 
{
  using namespace boost::process;
  using namespace boost::process::initializers;
  using namespace boost::iostreams;

  boost::filesystem::path command = boost::process::search_path(settings.objdump());

  std::vector<std::string> args;

  args.push_back( command.string() );

  // dump symbols"
  args.push_back( "-t" );

  // demangle
  args.push_back( "-C" );

  // only include symbols in the ".text" segment
  args.push_back( "-j" ); args.push_back( ".text" );

  // look in the object
  args.push_back( object.string() );

  boost::process::pipe p = create_pipe();
  file_descriptor_sink sink(p.sink, close_handle);
  
  child c = execute( set_args(args) , bind_stdout( sink ) , close_stderr() );
  auto exit_code = wait_for_exit(c);
  sink.close();

  file_descriptor_source source(p.source, close_handle);
  stream<file_descriptor_source> is(source);
  std::string s;
  std::string const pattern = "[.text] ";
  std::vector<std::string> result;
  while( is && !is.eof() )
    {
      std::getline(is, s);
      std::string::size_type pos = s.find(pattern);
      if ( pos != std::string::npos )
	{
	  std::string method = s.substr(pos + pattern.length());
	  result.push_back(method);
	}
    }

  return result;
}
