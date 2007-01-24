#ifndef DEPLOYMENT_H
#define DEPLOYMENT_H

/*
 *  Deployment.h
 *  
 *  Created by David Bergman on 1/23/07.
 */

#include <string>

namespace cpsh {

/**
 * Singleton class holding all deployment-specific information,
 * such as what compiler to use
 */
class Deployment {
public:
	static Deployment& instance();

	/**
	 * Is it MSVC, gcc etc.
	 */
	std::string getCompilerTool() const;
	/**
	 * Is it win32, unix etc.
	 */
	std::string getPlatform() const;
	
	/**
	 * Should we include the Boost headers?
	 */
	bool useBoost() const;

	void setUseBoost(bool flag);
	
  static const std::string TOOL_MSVC;
  static const std::string TOOL_GCC;
  static const std::string PLATFORM_WIN32;
  static const std::string PLATFORM_UNIX;
protected:
	Deployment();
	
	void guessEnvironment();
private:
	/**
	 * The compiler tool to use
	 */
	std::string compilerTool_;
	
	/**
	 * The platform/OS used
	 */
	std::string platform_;
	
	/**
	 * Are we actually using Boost?
	 * This flag can be set programmatically, but is
	 * defaulted to true iff the BOOST_HOME variable
	 * is set
	 */
	bool useBoost_;

	static Deployment* singleton_;
};

} // namespace cpsh

#endif
