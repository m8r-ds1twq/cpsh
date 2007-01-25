/*

File: cpsh.cpp
Author: David Bergman

This is a C++ shell and script language implementation.
I call the language CPScript, which is mainly C++
with all the goodies prepared for you, which is currently
Boost but most of it will be TR1. Also, both the namespaces
'std' and 'boost' are imported to the script.

To start it as a shell, just issue the "-i" option, else
it will compile the standard input CPScript

*/

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "Script.h"

using namespace cpsh;

int main(int argc, const char* argv[])
{
  bool interactive = argc > 1 && std::string(argv[1]) == "-i";
  std::string scriptPath;
  if (argc > 1 && !interactive)
    scriptPath.assign(argv[1]);
  Script script(interactive);
  // We create exe parameters for all remaining parameters
  std::vector<std::string> params(argv+2, argv+std::max(2, argc));
  script.setExeParameters(params);
  // Due to some problems with GCC 4.0.3 (not with GCC 3.4...)
  // regarding copy constructor semantics for the tertiary
  // operator (and basic_streambuf being private) we
  // cannot do it the elegant way
  if (scriptPath.empty())
    std::cin >> script;
  else {
    std::ifstream inFile(scriptPath.c_str());
    inFile >> script;
  }
  try {
    script.compile();
    script.execute();  
  }
  catch (const std::runtime_error& ex) {
    std::cout << "# Problem. " << ex.what() << std::endl;
  }
}
