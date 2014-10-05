#include "settings.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>

namespace fab
{
  settings::settings()
    : cxx_compiler_("g++-4.9")
    , c_compiler_("gcc-4.9")
    , objdump_("gobjdump")
    , build_all_(false)
  { }

  settings::path_type settings::fab() const
  { return fab_; }

  settings& settings::fab(path_type const& value) 
  { 
    fab_ = value; 
    return *this;
  }

  std::set< settings::path_type > const& settings::ignore( ) const
  { return ignore_; }

  settings& settings::ignore( std::set< path_type > const& value )
  {
    ignore_ = value;
    return *this;
  }

  settings& settings::ignore_insert( path_type const& path )
  {
    ignore_.insert(path);
    return *this;
  }

  std::string settings::target() const 
  { 
    return target_; 
  }

  settings& settings::target( std::string const& value ) 
  { 
    target_ = value; 
    return *this; 
  } 

  std::string settings::type() const 
  { 
    return type_; 
  }

  settings& settings::type( std::string const& value ) 
  { 
    std::cout << "settings.type = \"" << value << "\"" << std::endl;
    type_ = value; 
    return *this; 
  } 

  settings& settings::deserialize(boost::property_tree::ptree const& source)
  {
    std::string new_target = source.get_child("target",boost::property_tree::ptree()).data();
    target(new_target);

    std::string new_build_all = source.get_child("build_all",boost::property_tree::ptree()).data();
    if ( new_build_all.length() )
      build_all(boost::lexical_cast<bool>(new_build_all));

    std::string new_type = source.get_child("type",boost::property_tree::ptree()).data();
    if ( new_type.length())
      type(new_type);

    boost::property_tree::ptree ignore = source.get_child("ignore",boost::property_tree::ptree());
    for( auto i = ignore.begin() ; i != ignore.end() ; ++i )
      ignore_insert(i->second.data());

    boost::property_tree::ptree cxxflags = source.get_child("cxxflags",boost::property_tree::ptree());
    for( auto i = cxxflags.begin() ; i != cxxflags.end() ; ++i )
      cxxflags_.insert(i->second.data());

    boost::property_tree::ptree cflags = source.get_child("cflags",boost::property_tree::ptree());
    for( auto i = cflags.begin() ; i != cflags.end() ; ++i )
      cflags_.insert(i->second.data());

    boost::property_tree::ptree lflags = source.get_child("lflags",boost::property_tree::ptree());
    for( auto i = lflags.begin() ; i != lflags.end() ; ++i )
      lflags_.insert(i->second.data());

    boost::property_tree::ptree library_dirs  = source.get_child("library_dirs",boost::property_tree::ptree());
    for( auto i = library_dirs.begin() ; i != library_dirs.end() ; ++i )
      library_dirs_.insert(i->second.data());

    boost::property_tree::ptree library_files  = source.get_child("library_files",boost::property_tree::ptree());
    for( auto i = library_files.begin() ; i != library_files.end() ; ++i )
      library_files_.insert(i->second.data());
  }

  std::set< settings::path_type > const& settings::source_modules() const
  {
    return source_modules_;
  }

  settings& settings::source_modules(std::set< path_type > const& value)
  {
    source_modules_ = value;
    return *this;
  }

  settings& settings::source_modules_insert( path_type const& path )
  {
    source_modules_.insert(path);
    return *this;
  }

  std::set< settings::path_type > const& settings::library_dirs() const
  {
    return library_dirs_;
  }

  settings& settings::library_dirs(std::set< path_type > const& value)
  {
    library_dirs_ = value;
    return *this;
  }

  settings& settings::library_dirs_insert( path_type const& path )
  {
    library_dirs_.insert(path);
    return *this;
  }

  std::set< settings::path_type > const& settings::library_files() const
  {
    return library_files_;
  }

  settings& settings::library_files(std::set< path_type > const& value)
  {
    library_files_ = value;
    return *this;
  }

  settings& settings::library_files_insert( path_type const& path )
  {
    library_files_.insert(path);
    return *this;
  }

  std::set< settings::path_type > const& settings::objects() const
  {
    return objects_;
  }

  settings& settings::objects(std::set< path_type > const& value)
  {
    objects_ = value;
    return *this;
  }

  settings& settings::objects_insert( path_type const& path )
  {
    objects_.insert(path);
    return *this;
  }


  std::set<std::string> const& settings::cxxflags() const
  {
    return cxxflags_;
  }

  settings& settings::cxxflags(std::set<std::string> const& value)
  {
    cxxflags_ = value;
    return *this;
  }

  settings& settings::cxxflags_insert(std::string const& value)
  {
    cxxflags_.insert(value);
    return *this;
  }

  std::set<std::string> const& settings::cflags() const
  {
    return cflags_;
  }

  settings& settings::cflags(std::set<std::string> const& value)
  {
    cflags_ = value;
    return *this;
  }

  settings& settings::cflags_insert(std::string const& value)
  {
    cflags_.insert(value);
    return *this;
  }

  std::set<std::string> const& settings::lflags() const
  {
    return lflags_;
  }

  settings& settings::lflags(std::set<std::string> const& value)
  {
    lflags_ = value;
    return *this;
  }

  settings& settings::lflags_insert(std::string const& value)
  {
    lflags_.insert(value);
    return *this;
  }

  bool settings::build_all() const
  {
    return build_all_;
  }

  settings& settings::build_all(bool value)
  {
    build_all_ = value;
    return *this;
  }

  std::string settings::cxx_compiler() const
  {
    return cxx_compiler_;
  }

  settings& settings::cxx_compiler(std::string const& value)
  {
    cxx_compiler_ = value;
    return *this;
  }

  std::string settings::c_compiler() const
  {
    return c_compiler_;
  }

  settings& settings::c_compiler(std::string const& value)
  {
    c_compiler_ = value;
    return *this;
  }

  std::string settings::objdump() const
  {
    return objdump_;
  }

  settings& settings::objdump(std::string const& value)
  {
    objdump_ = value;
    return *this;
  }

}
