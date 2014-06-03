#include "scan_test_dir.hpp"

#include "build_target.hpp"
#include "compile_module.hpp"
#include "failure.hpp"
#include "object_list_functions.hpp"

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/process.hpp> 
#include <boost/process/mitigate.hpp>

#include <iostream>
#include <fstream>

void run_tests( boost::filesystem::path test_suite )
{
  using namespace boost::process;
  using namespace boost::process::initializers;

  std::vector<std::string> args;

  args.push_back(test_suite.string());

  std::cout << "Testing\n - ";
  std::copy( args.begin(), args.end() , std::ostream_iterator<std::string>(std::cout, " "));
  std::cout << std::endl;

  child c = execute( set_args(args) );
  auto exit_code = wait_for_exit(c);
  if (BOOST_PROCESS_EXITSTATUS(exit_code))
    {
      throw failure( BOOST_PROCESS_EXITSTATUS(exit_code), "Tests failed on module:" + test_suite.string() );
    }


}

void list_test_methods( boost::filesystem::path test_object, std::vector<std::string>& test_methods, fab::settings& settings ) 
{
  std::vector<std::string> functions = object_list_functions(settings,test_object);
  for( auto fn = functions.begin() ; fn != functions.end() ; ++fn )
    {
      if ( fn->substr(0,4) == "test" && fn->substr(fn->length()-2) == "()" )
	{
	  test_methods.push_back(fn->substr(0,fn->length()-2));
	}
    }
}

void scan_test_dir_entry(std::set<boost::filesystem::path>& test_objects, std::vector<std::string>& test_methods, fab::settings& settings, boost::filesystem::directory_entry entry)
{
  boost::filesystem::path extension = entry.path().extension();
  if (!(extension == ".C" || extension == ".cpp" || extension == ".cxx" || extension == ".cc"))
    return;

  std::cout << "- test module:" << entry.path() << std::endl;
  boost::filesystem::path test_object = compile_module(settings,entry);

  test_objects.insert(test_object);
  list_test_methods( test_object , test_methods , settings );
}

class ostream_iterator
{
public:
  class writer
  {
  public:
    writer( std::ostream& os , std::string delimiter )
      : os_(os)
      , delimiter_(delimiter)
    {}

    template< typename V >
    writer& operator=( V const& v)
    {
      os_ << del_ << v;
      del_ = delimiter_;
      return *this;
    }

  private:
    std::ostream& os_;
    std::string delimiter_;
    std::string del_;
  };

  ostream_iterator( std::ostream& os , std::string delimiter )
    : writer_(os,delimiter)
  { }

  writer& operator*()
  {
    return writer_;
  }

  ostream_iterator& operator++()
  {
    return *this;
  }

private:
  writer writer_;
};

namespace std 
{
  template<> class iterator_traits<::ostream_iterator> 
  {
  public:
    typedef ::ostream_iterator::writer value_type;
  };
}

void write_test_suite( std::vector< std::string > const& test_methods )
{
  std::ofstream test_suite( ".test_suite.cpp" );
  test_suite << "// automatically generated\n";
  test_suite << "#include <iostream>\n";

  for( auto fn = test_methods.begin() ; fn != test_methods.end() ; ++fn )
    {
      test_suite << "extern void " << *fn << "();\n";
    }

  test_suite << "typedef void (*test_fn)();\n\n";

  test_suite << "struct test\n"
    " {\n"
    " test_fn fn_;\n"
    " char const* name_;\n"
    " };\n\n";

  test_suite << "test tests[] =";
  std::string delim = "\n { ";
  for( auto method : test_methods )
    {
      test_suite << delim;
      test_suite << "{ " << method << " , \"" << method << "\" }";
      delim = "\n , ";
    }
  test_suite << " };\n\n";

  test_suite << 
    "int main(int argc, char**argv)\n"
    "{\n"
    "  int fail_count = 0;\n"
    "  int pass_count = 0;\n"
    "  for( auto test : tests )\n"
    "    {\n"
    "      std::cout << \"--- \\\"\" << test.name_ << \"\\\"\" << std::endl;\n"
    "      try\n"
    "        {\n"
    "          test.fn_();\n"
    "          std::cout << \"test \\\"\" << test.name_ << \"\\\" PASSED\" << std::endl;\n"
    "          ++pass_count;\n"
    "        }\n"
    "      catch( std::exception& e )\n"
    "        {\n"
    "          std::cerr << \"test \\\"\" << test.name_ << \"\\\" FAILED\" << std::endl;\n"
    "          ++fail_count;\n"
    "        }\n"
    "    }\n"
    "  std::cout << pass_count << \" tests PASSED\" << std::endl;\n"
    "  std::cout << fail_count << \" tests FAILED\" << std::endl;\n"
    "  return fail_count != 0;\n"
    "}\n";
}

void scan_test_dir(fab::settings& settings, boost::filesystem::path dir)
{
  using boost::filesystem::recursive_directory_iterator;

  std::set<boost::filesystem::path> test_objects;
  std::vector<std::string> test_methods;

  for( auto iter = recursive_directory_iterator(dir) ; iter != recursive_directory_iterator() ; ++iter )
    {
      scan_test_dir_entry(test_objects,test_methods,settings, *iter);
    }

  write_test_suite( test_methods );

  boost::filesystem::path main_object = compile_module(settings,".test_suite.cpp");
  std::copy(settings.objects().begin(),settings.objects().end(),std::inserter(test_objects,test_objects.begin()));

  build_target( settings, ".test_suite.exe", test_objects ,  main_object);

  run_tests( boost::filesystem::path("./.test_suite.exe" ));
}

