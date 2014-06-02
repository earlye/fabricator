#ifndef hCE54937C_11D2_4362_87B9_7BE351AA9469
#define hCE54937C_11D2_4362_87B9_7BE351AA9469

#include <set>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

namespace fab
{
  class settings
  {
  public:
    typedef boost::filesystem::path path_type;

    settings();

    path_type fab() const;
    settings& fab(path_type const& value);

    std::set< path_type > const& source_modules() const;
    settings& source_modules(std::set< path_type > const&);
    settings& source_modules_insert( path_type const& path );

    std::set< path_type > const& library_dirs() const;
    settings& library_dirs(std::set< path_type > const&);
    settings& library_dirs_insert( path_type const& path );

    std::set< path_type > const& objects() const;
    settings& objects(std::set< path_type > const&);
    settings& objects_insert( path_type const& path );

    std::set< path_type > const& ignore( ) const;
    settings& ignore( std::set< path_type > const& value );
    settings& ignore_insert( path_type const& path );

    std::string target() const;
    settings& target( std::string const& value );

    settings& deserialize( boost::property_tree::ptree const& source );

    std::set<std::string> const& cxxflags( ) const;
    settings& cxxflags( std::set<std::string> const& value );
    settings& cxxflags_insert( std::string const& value );

    std::set<std::string> const& lflags( ) const;
    settings& lflags( std::set<std::string> const& value );
    settings& lflags_insert( std::string const& value );

    bool build_all() const;
    settings& build_all( bool value );

    std::string compiler() const;
    settings& compiler(std::string const& value);

    std::string objdump() const;
    settings& objdump(std::string const& value);

  private:
    std::set< path_type > ignore_;
    std::set< path_type > source_modules_;
    std::set< path_type > library_dirs_;
    std::set< path_type > objects_;
    std::set< std::string > cxxflags_;
    std::set< std::string > lflags_;
    path_type fab_;
    std::string target_;
    std::string objdump_;
    std::string compiler_;
    bool build_all_;
  };
}

#endif
