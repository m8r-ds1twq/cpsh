/*

File: Line.cpp
Author: David Bergman

*/

#include <iostream>

#include "Line.h"

namespace cpsh {

Line::Line()
{
  clear();
}

Line::Line(const std::string& line)
{
  clear();
  *this << line;
}

void Line::clear()
{
  lineType_ = LINE_NONE;
  std::string empty;
  std::swap(text_, empty);
}

Line& Line::operator<<(const std::string& line)
{
  bool setType = text_.empty();
  extractLine(line, setType);
  return *this;
}

Line::Type Line::getType() const
{
  return lineType_;
}

std::string Line::getText(bool withEOL) const
{
  return withEOL ? text_ + "\n" : text_;
}

std::string Line::stripContinuation(const std::string& line,
				    bool& hadContinuation)
{
  hadContinuation = !line.empty() &&
    line[line.length()-1] == '\\';
  return hadContinuation ? line.substr(0, line.length()-1) : line;
}

void Line::extractLine(const std::string& line,
		       bool setType)
{
  if (setType)
    decideType(line);
  if (!line.empty()) {
    int startIndex = !setType || lineType_ == LINE_STAT ? 0 : 1;
    text_ += line.substr(startIndex);
  }
}

void Line::decideType(const std::string& line)
{
  // We decide the type of line based on the first character
  if (line.empty())
    lineType_ = LINE_NONE;
  else {
    char firstC = line[0];
    switch (firstC) {
    case '.':
      lineType_ = LINE_DEF; break;
    case '?':
      lineType_ = LINE_QUERY; break;
    case ':':
      lineType_ = LINE_ADMIN; break;
    case '#':
      lineType_ = LINE_COMMENT; break;
    default:
      lineType_ = LINE_STAT; break;
    }
  }
}

} // namespace cpsh
