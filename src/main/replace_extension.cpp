#include "replace_extension.hpp"

boost::filesystem::path replace_extension( boost::filesystem::path source , boost::filesystem::path const& new_extension )
{
  source.replace_extension( new_extension );
  return source;
}
