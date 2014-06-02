#include "contains_main.hpp"

#include "settings.hpp"

#include <boost/iostreams/stream.hpp>
#include <boost/process.hpp> 
#include <boost/process/mitigate.hpp>

bool contains_main( fab::settings& settings, boost::filesystem::path object ) 
{
  using namespace boost::process;
  using namespace boost::process::initializers;
  using namespace boost::iostreams;

  boost::filesystem::path command = boost::process::search_path(settings.objdump());

  std::vector<std::string> args;

  args.push_back( command.string() );

  // dump symbols"
  args.push_back( "-t" );

  // only include symbols in the ".text" segment
  args.push_back( "-j" ); args.push_back( ".text" );

  // look in the object
  args.push_back( object.string() );

  //std::cout << "Checking " << object.string() << " for main()\n - ";
  //std::copy( args.begin(), args.end() , std::ostream_iterator<std::string>(std::cout, " "));
  //std::cout << std::endl;

  boost::process::pipe p = create_pipe();
  file_descriptor_sink sink(p.sink, close_handle);
  
  child c = execute( set_args(args) , bind_stdout( sink ) , close_stderr() );
  auto exit_code = wait_for_exit(c);
  sink.close();

  file_descriptor_source source(p.source, close_handle);
  stream<file_descriptor_source> is(source);
  std::string s;
  std::string const pattern = "[.text] _main";
  while( is && !is.eof() )
    {
      std::getline(is, s);
      int pos = s.find(pattern);
      int epos = s.length() - pattern.length();
      if ( epos >= 0 && pos > 0 && epos == pos )
	{
	  return true;
	}
    }

  return false;
}
