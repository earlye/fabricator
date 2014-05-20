#include "read_json.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/property_tree/json_parser.hpp>

boost::property_tree::ptree read_json(boost::filesystem::path path)
{
  boost::filesystem::ifstream file(path);
  boost::property_tree::ptree json;
  boost::property_tree::read_json(file,json);
  return json;  
}
