#include "failure.hpp"
#include "try_main.hpp"

#include <iostream>

// Thunk down to try_main, translate failure exceptions to exit codes. Other exceptions don't have a failure_code(), so just
// let c++ do what it normally does.
int main(int argc, char** argv )
{
  try
    {
      try_main(argc, argv);
      return 0;
    }
  catch( failure f )
    {
      std::cerr << f.what() << std::endl;
      return f.failure_code();
    }
}
