/*

File: CodeTemplate.cpp
Author: David Bergman

Defines the code template wrapping the script

*/

#include "CodeTemplate.h"

namespace cpsh {

const std::string CodeTemplate::STD_HEADER = "\
#include <cstdio>\n\
#include <cstdlib>\n\
#include <string>\n\
#include <iostream>\n\
#include <fstream>\n\
#include <sstream>\n\
#include <algorithm>\n\
#include <numeric>\n\
#include <vector>\n\
#include <list>\n\
using namespace std;\n";

const std::string CodeTemplate::BOOST_HEADER = "\
#include <boost/iterator.hpp>\n\
#include <boost/array.hpp>\n\
#include <boost/assert.hpp>\n\
#include <boost/range.hpp>\n\
#include <boost/type_traits.hpp>\n\
#include <boost/variant.hpp>\n\
#include <boost/weak_ptr.hpp>\n\
#include <boost/timer.hpp>\n\
#include <boost/spirit.hpp>\n\
#include <boost/spirit.hpp>\n\
#include <boost/lexical_cast.hpp>\n\
#include <boost/shared_ptr.hpp>\n\
#include <boost/tuple/tuple.hpp>\n\
#include <boost/function.hpp>\n\
#include <boost/lambda/lambda.hpp>\n\
#include <boost/lambda/bind.hpp>\n\
#include <boost/random.hpp>\n\
#include <boost/regex.hpp>\n\
\n\
using namespace boost;\n\
using namespace boost::lambda;\n\
\n";

const std::string CodeTemplate::DEFAULT_MAIN_PRE = "\
int main(int argc, char* argv[])\n\
{\n\
\n";

const std::string CodeTemplate::DEFAULT_MAIN_POST = "\n\
}\n\
\n";

const std::string& CodeTemplate::getStdHeader()
{
  return STD_HEADER;
}

const std::string& CodeTemplate::getBoostHeader()
{
  return BOOST_HEADER;
}

const std::string& CodeTemplate::getMainPre()
{
  return DEFAULT_MAIN_PRE;
}

const std::string& CodeTemplate::getMainPost()
{
  return DEFAULT_MAIN_POST;
}

} // namespace cpsh
