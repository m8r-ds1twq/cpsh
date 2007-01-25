/*

File: Script.h
Author: David Bergman

A CPScript abstraction

*/

#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <stdexcept>

#include "Script.h"
#include "CodeTemplate.h"
#include "Deployment.h"

namespace cpsh {

  const std::string Script::DEFAULT_CPP_FILE = "temp.cpp";
  const std::string Script::DEFAULT_EXE_FILE = "temp.exe";
  const std::string Script::DEFAULT_ERR_FILE = "err.log";
  const std::string Script::DEFAULT_OUT_FILE = "out.log";
  const std::string Script::DEFAULT_QUERY_FILE = "query.log";

/**
 * Create an empty script, either as an interactive session
 * or a one-shot script
 */
Script::Script(bool interactive)
  : interactive_(interactive),
    mainPre_(CodeTemplate::getMainPre()),
    mainPost_(CodeTemplate::getMainPost()),
		mainCode_(std::ostringstream::app),
		lastGoodMainCode_(std::ostringstream::app),
		definitionCode_(std::ostringstream::app),
		lastGoodDefinitionCode_(std::ostringstream::app)
{
  if (interactive_)
    std::cout << "> ";
}

/**
 * Get the parameters passed to the script,
 * as a joined string
 */
std::string Script::getExeParameters() const
{
  std::ostringstream out;
  std::copy(exeParams_.begin(), exeParams_.end(),
	    std::ostream_iterator<std::string>(out, " "));
  return out.str();
}

void Script::setExeParameters(const std::vector<std::string>& params)
{
  exeParams_ = params;
}

void Script::readInput(std::istream& input)
{
  // We make sure to gather input with a trailing
  // '/' into one chunk
  char buffer[1000];
  Line lastInput;
  while (!input.eof()) {
    input.getline(buffer, sizeof(buffer));
    bool multiLine;
    lastInput << Line::stripContinuation(buffer,
					 multiLine);
    if (!multiLine) {
      try {
	addLine(lastInput);
	lastInput.clear();
      }
      catch (const std::runtime_error& ex) {
	lastInput.clear();
	if (!interactive_)
	  throw;
	std::cout << "# Problem. " << ex.what()
		  << ". Error output: "
		  << getError() << std::endl;
      }
    }
    if (interactive_)
      std::cout << (multiLine ? ">> " : "> ");   
  }
}

/**
 * Add a line of code to the current context.
 */
void Script::addLine(const Line& line)
{
  std::remove(DEFAULT_ERR_FILE.c_str());
  std::remove(DEFAULT_OUT_FILE.c_str());
  std::remove(DEFAULT_QUERY_FILE.c_str());
  query_ = "";

  // We have four kinds of input
  
  bool shouldRecompile = true;
  switch (line.getType()) {
  case Line::LINE_ADMIN:
    execAdmin(line.getText()); shouldRecompile = false; break;
  case Line::LINE_STAT:
    mainCode_ << line.getText(true); break;
  case Line::LINE_DEF:
    definitionCode_ << line.getText(true); break;
  case Line::LINE_QUERY:
    query_ = line.getText(true); break;
  case Line::LINE_COMMENT:
  case Line::LINE_NONE:
    shouldRecompile = false; break;
  default:
    std::cout << "# Error. Strange line type in \"" <<
      line.getText() << "\"";
    shouldRecompile = false;
  }
  if (interactive_ && shouldRecompile) {      
    compile();
    execute();
    // Let us output the text of the execution,
    // or the query...
    std::string out = getOutput();
    std::string query = getQueryOutput();
    if (!out.empty())
      std::cout << "{" << out << "}\n";
    if (!query.empty())
      std::cout << "< " << query << '\n';
  }
}

void Script::compile()
{
  outputSource(DEFAULT_CPP_FILE);
  try {
    compileSource(DEFAULT_CPP_FILE, DEFAULT_EXE_FILE);
    // removeSource(DEFAULT_CPP_FILE);
    // Make sure to remember this good compilation
    lastGoodDefinitionCode_.str(definitionCode_.str());
    lastGoodMainCode_.str(mainCode_.str());
  }
  catch (...) {
    // We rollback to the last good code
    definitionCode_.str(lastGoodDefinitionCode_.str());
    mainCode_.str(lastGoodMainCode_.str());
    throw;
  }  
}

void Script::execute()
{
  runExe(DEFAULT_EXE_FILE);
  removeExe(DEFAULT_EXE_FILE);
}

void Script::execAdmin(const std::string& cmd)
{
  if (cmd == "c") {
    mainCode_.str("");
    lastGoodDefinitionCode_.str("");
  } else if (cmd == "cd") {
    definitionCode_.str("");
    lastGoodDefinitionCode_.str("");
  } else if (cmd == "l") {
    std::cout << "# code: \n" << mainCode_.str();
  } else if (cmd == "ld") {
    std::cout << "# def: \n" << definitionCode_.str();
  } else if (cmd == "b-") {
		Deployment::instance().setUseBoost(false);
    std::cout << "# disabled Boost";
	} else if (cmd == "b+") {
		Deployment::instance().setUseBoost(true);
		std::cout << "# enabled Boost";
	} else {
      std::cout << "# possible commands: \n"
	"#  :c  = clear main code\n"
	"#  :cd = clear definitions\n"
	"#  :l  = list main code\n"
	"#  :ld = list definitions";
	}
}

void Script::outputSource(const std::string& sourcePath)
{
  std::ofstream output(sourcePath.c_str());
  output << getHeader()
	 << "\n#line 1 \"definitions\"\n"
	 << definitionCode_.str()
	 << mainPre_
	 << "\n#line 1 \"main\"\n"
	 << mainCode_.str()
	 << "\n#line 1 \"query\"\n"
	 << getQueryOutputter()
	 << mainPost_;
}

std::string Script::getQueryOutputter() const
{
  return query_.empty() ? "" : 
    "{ ofstream out__(\"" + DEFAULT_QUERY_FILE +
    "\"); out__ << " + query_ + "; }";
}

void Script::removeSource(const std::string& sourcePath)
{
  std::remove(sourcePath.c_str());
}

void Script::removeExe(const std::string& exePath)
{
  std::remove(exePath.c_str());
}

void Script::compileSource(const std::string& sourcePath,
			   const std::string& exePath)
{
  // We support either MSVC or GCC right now
  std::string tool = Deployment::instance().getCompilerTool();
  std::string platform = Deployment::instance().getPlatform();
  bool useMsvc = tool == Deployment::TOOL_MSVC;
  bool isWin = platform == Deployment::PLATFORM_WIN32;
  const std::string cmd =
    (useMsvc ? "cl /EHsc /w /nologo /Fe" : "g++ -o ")
    + exePath +
    (useMsvc ? " /I" : " -I ") +
    (isWin ? "\"%BOOST_HOME%\" " :  "\"$BOOST_HOME\" ") +
    sourcePath + " > " + DEFAULT_ERR_FILE;
//		 + " 2>&1";
  int compileStatus = std::system(cmd.c_str());
  if (compileStatus)
    throw std::runtime_error("Compilation error");
}

void Script::runExe(const std::string& exePath)
{
	bool isUnix = Deployment::instance().getPlatform() == Deployment::PLATFORM_UNIX;
  std::string cmd = 
		(isUnix ? "./" : "") +
		exePath + ' '
    + getExeParameters();
  if (interactive_)
    cmd += " > "
      + DEFAULT_OUT_FILE
      + " 2> "
      + DEFAULT_ERR_FILE;
 
  int execStatus = std::system(cmd.c_str()); 
  if (execStatus)
    throw std::runtime_error("Execution error");
}

std::string Script::getError() const
{
  return getFileText(DEFAULT_ERR_FILE);
}

std::string Script::getOutput() const
{
  return getFileText(DEFAULT_OUT_FILE);
}

std::string Script::getQueryOutput() const
{
  return getFileText(DEFAULT_QUERY_FILE);
}

std::string Script::getFileText(const std::string& filePath) const
{
  std::ostringstream tempStream;
  std::ifstream errStream(filePath.c_str());
  while (errStream.good() && !errStream.eof())
    tempStream << static_cast<char>(errStream.get());
  std::string data = tempStream.str();
  std::string::size_type endNonWhite =
    data.find_last_not_of(' ');
  if (endNonWhite != std::string::npos)
    data.erase(endNonWhite);
  return data;
}

void Script::clearError()
{
  std::remove(DEFAULT_ERR_FILE.c_str());
}

/**
 * Get the full header portion, which includes the
 * the standard include statements and, if enabled,
 * the Boost inclusions. It also adds a private
 * header.
 */
std::string Script::getHeader() const
{
  return CodeTemplate::getStdHeader() +
    (Deployment::instance().useBoost() ? CodeTemplate::getBoostHeader() : "") +
    myHeader_.str();
}

std::istream& operator>>(std::istream& stream, Script& script)
{
  script.readInput(stream);
  return stream;
}

} // namespace cpsh
