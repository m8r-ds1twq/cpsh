#ifndef SCRIPT_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <boost/variant/variant.hpp>

#include "Line.h"

namespace cpsh {

/**
 * This class represents scripts - and sessions.
 * Its responsibility is to maintain the state of
 * execution and issue the proper compilation and
 * execution under the hood.
 * @author David Bergman
 */
class Script {
 public:

  Script(bool interactive = false);
  void setExeParameters(const std::vector<std::string>& params);
  void readInput(std::istream& stream);
  void addLine(const Line& line);
  void compile();
  void execute();

 protected:

  void outputSource(const std::string& sourcePath);
  void removeSource(const std::string& sourcePath);
  void removeExe(const std::string& exePath);
  void compileSource(const std::string& sourcePath, 
		     const std::string& exePath);
  void runExe(const std::string& exePath);
  std::string getExeParameters() const;
  std::string getError() const;
  std::string getOutput() const;
  std::string getQueryOutput() const;
  std::string getFileText(const std::string& filePath) const;
  void clearError();
  
  /**
   * Get a proper output statement for the current
   * expression query
   */
  std::string getQueryOutputter() const;

  /**
   * Get the header portion
   */
  std::string getHeader() const;

  void execAdmin(const std::string& cmd);

 private:
  std::ostringstream mainCode_;
  std::ostringstream lastGoodMainCode_;
  std::ostringstream definitionCode_;
  std::ostringstream lastGoodDefinitionCode_;

  // Query if any
  std::string query_;
  
  // My private header part
  // NOTE: currently not alterable...
  std::ostringstream myHeader_;
  std::string mainPre_;
  std::string mainPost_;

  // The execution command-line parameters
  // for the script
  std::vector<std::string> exeParams_;

  /**
   * Saved error output
   */
  std::string error_;

  /**
   * Are we in the interactive environment?
   */
  bool interactive_;

  static const std::string STD_HEADER;
  static const std::string BOOST_HEADER;
  static const std::string DEFAULT_MAIN_PRE;
  static const std::string DEFAULT_MAIN_POST;
  static const std::string DEFAULT_CPP_FILE;
  static const std::string DEFAULT_EXE_FILE;
  static const std::string DEFAULT_ERR_FILE;
  static const std::string DEFAULT_OUT_FILE;
  static const std::string DEFAULT_QUERY_FILE;
};

std::istream& operator>>(std::istream& stream, Script& script);

} // namespace cpsh

#endif
