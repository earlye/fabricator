#include <iostream>
#include <stdexcept>

void test_foo() 
{
  std::cout << "hello world!" << std::endl;
}


void test_fail()
{
  try
    {
      throw std::runtime_error("failure");
    }
  catch( std::runtime_error& e )
    {
      std::cout << "Caught expected error: " << e.what() << std::endl;
    }
}
