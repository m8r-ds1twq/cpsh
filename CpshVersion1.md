# Introduction #

The first official release was added 1/23/07. This is version 1.0. This page describes specifics of that version.

# Building #

The only make systems supported directly are XCode for Mac OS X and Visual Studio 2005 for Windows.

## Requirements ##

A standard compliant C++ compiler and Boost, with the environment variable `BOOST_HOME` set properly. In order to use the provided make files, you need either Visual Studio 2005 or XCode 2.x.

## OS X ##

Open the XCode 2.4 project file and build it...

Problems:
  * it currently uses the path `/usr/local/boost_1_33_1` explicitly in the XCode project

## Windows ##

Open the Visual Studio 2005 project file and build it...

# Running #

Just make sure cpsh is in the search path and issue

```
cpsh < my_script.cpp
```

where `my_script.cpp` is a script file, or

```
cpsh -i
```

to start an interactive session.

## Requirements ##

MSVC or gcc installed. If the environment variable `MSVCDir` is set, it will assume MSVC (`cl`), else it will default to `gcc`. If you want support for Boost, you need to have the environment variable `BOOST_HOME` set properly. You can always turn on or off Boost support in runtime with the command `:b+` and `:b-`.