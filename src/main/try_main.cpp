#include "try_main.hpp"

#include "build_target.hpp"
#include "contains.hpp"
#include "contains_main.hpp"
#include "failure.hpp"
#include "read_json.hpp"
#include "scan_child.hpp"
#include "scan_source_dir.hpp"
#include "scan_test_dir.hpp"
#include "compile_module.hpp"
#include "settings.hpp"

#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <string>
#include <stdexcept>

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/program_options.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

void display_version(bool value)
{
  if ( value ) 
    {
      throw failure(0,"Fabricator Version 0.3");
    }
}

void try_main(int argc, char** argv)
{
  po::options_description options("Fabricator Options");
  options.add_options()
    ("help", "produce help message")
    ("dir" , po::value<std::string>(), "build in directory. If not provided, builds in current directory.")
    ("version" , po::bool_switch()->notifier(&display_version), "display version" )
    ;

  po::variables_map config;
  po::store(po::parse_command_line(argc,argv,options),config);
  po::notify(config);

  if (config.count("help"))
    {
      std::cout << options << std::endl;
      return;
    }

  boost::filesystem::path build_dir = boost::filesystem::current_path();
  if (config.count("dir"))
    {
      build_dir = boost::filesystem::path(config["dir"].as<std::string>());
    }
  std::cout << "Entering directory `" << build_dir.string() << "'" << std::endl;
  boost::filesystem::current_path( build_dir );

  fab::settings settings;
  fs::path fab_json_path("Fab.json");
  if (fs::exists(fab_json_path))
    {
      settings.deserialize(read_json(fab_json_path));
    }
  settings.fab(argv[0]);

  // If a Fab file doesn't have a "target" node, the presumption is that this is a directory full of components to build.
  if (settings.target().empty()) 
    {
      fs::path dir_to_scan(".");  
      std::for_each(fs::directory_iterator(dir_to_scan), fs::directory_iterator(),boost::bind(scan_child,settings,_1));
    }
  else
    {
      // Find source modules.
      // For now, this means "scan src/main" and "scan library_dirs". "library_dirs" will eventually go away, in favor of being able to specify dependencies, once artifact repos are set up.
      boost::filesystem::path src_main("src/main");
      scan_source_dir(settings,src_main);
      std::for_each(settings.library_dirs().begin(),settings.library_dirs().end(),boost::bind(scan_source_dir,boost::ref(settings),_1));      
      
      // Compile source modules
      boost::filesystem::path main_object;
      for( auto iter = settings.source_modules().begin(); iter != settings.source_modules().end() ; ++iter ) {
	boost::filesystem::path object = compile_module(settings,*iter);
	if ( !contains_main( settings, object ) )
	  settings.objects_insert(object);
	else
	  main_object = object;
      }

      // Link
      build_target(settings,settings.target(),settings.objects(),main_object,settings.type());

      // Run unit tests
      boost::filesystem::path src_test("src/test");
      scan_test_dir(settings,src_test);
    }

  std::cout << "Leaving directory `" << build_dir.string() << "'" << std::endl;

}

