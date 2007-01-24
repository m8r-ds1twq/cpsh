/*
 *  Deployment.cpp
 *  cpsh
 *
 *  Created by David Bergman on 1/23/07.
 */

#include <cstdlib>
#include <iostream>

#include "Deployment.h"

namespace cpsh {

Deployment* Deployment::singleton_ = 0;

const std::string Deployment::TOOL_MSVC = "msvc";
const std::string Deployment::TOOL_GCC = "gcc";
const std::string Deployment::PLATFORM_WIN32 = "win32";
const std::string Deployment::PLATFORM_UNIX = "unix";

Deployment& Deployment::instance()
{
	if (!singleton_)
		singleton_ = new Deployment();
	return *singleton_;
}

Deployment::Deployment()
{
	guessEnvironment();
}

bool Deployment::useBoost() const
{
	return useBoost_;
}

void Deployment::setUseBoost(bool flag)
{
	useBoost_ = flag;
}

std::string Deployment::getPlatform() const
{
	return platform_;
}

std::string Deployment::getCompilerTool() const
{
	return compilerTool_;
}

void Deployment::guessEnvironment()
{
	// The platform is easy, if not WIN32 - according to compiler - it is Unix...

#ifdef WIN32
	platform_ = PLATFORM_WIN32;
#else
	platform_ = PLATFORM_UNIX;
#endif

	// Right now we use the MSVC iff the MSVCDir environment variable is set;
	// otherwise we use gcc...

	compilerTool_ = std::getenv("MSVCDir") ? TOOL_MSVC : TOOL_GCC;
	
	// Boost is only used, by default, if the BOOST_HOME variable is set
	
	const char* env = std::getenv("BOOST_HOME");
	useBoost_ = env;
}

} // namespace cpsh

