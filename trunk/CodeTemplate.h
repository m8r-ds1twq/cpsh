#ifndef CODE_TEMPLATE_H_

/*

File: CodeTemplate.h
Author: David Bergman

Contains the code templates "wrapping" the script

*/

#include <string>

namespace cpsh {

/**
 * This class provides the templates for the produced
 * code
 * @author David Bergman
 */
class CodeTemplate {
 public:
  static const std::string& getStdHeader();
  static const std::string& getBoostHeader();
  static const std::string& getMainPre();
  static const std::string& getMainPost();

 private:
  static const std::string STD_HEADER;
  static const std::string BOOST_HEADER;
  static const std::string DEFAULT_MAIN_PRE;
  static const std::string DEFAULT_MAIN_POST;
};

} // namespace cpsh

#endif
