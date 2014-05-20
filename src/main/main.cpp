#include "failure.hpp"
#include "try_main.hpp"

#include <iostream>



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
  // check for and read Fab.json file
  // if Fab.json file requires plugins, load them.
  // if Fab.json file contains "target", then:
  //   scan src/main folder - for each *.cpp,*.c:
  //     run cpp -M to get dependencies
  //     run make against the .d file to generate .o file
  //   scan src/test folder - for each *.cpp:
  //     run cpp -M to get dependencies
  //     run make against generated .d file to generate .o file
  //     generate test-harness.cpp & compile it.
  //     run test-harness
  // if Fab.json file did not contain "target", or Fab.json was not present:
  //   scan subfolders, skipping any listed in "ignore" array, for each folder:
  //     run "fab" in that folder.

