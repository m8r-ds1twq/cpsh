/*

File: Line.h
Author: David Bergman

Lines of script, part of the 'cpsh' project

This is a line of script, which can be one of
<ul>
<li>(top-level) definition</li>
<li>regular statement</li>
<li>query for the value of an expression</li>
<li>admin command starting with ':'</li>
</ul>

Furthermore, the lines can end with a continuation '/'
to denote multi-line input

*/

#ifndef LINE_H_

#include <string>

namespace cpsh {

class Line {
 public:
  Line();
  Line(const std::string& firstLine);
  void clear();
  Line& operator <<(const std::string& line);
  enum Type {
    LINE_NONE,
    LINE_STAT,
    LINE_DEF,
    LINE_QUERY,
    LINE_ADMIN,
    LINE_COMMENT
  };

  Line::Type getType() const;
  std::string getText(bool withEOL = false) const;

  /**
   * Strip continuation and also decides whether this
   * statement is continued
   */
  static std::string stripContinuation(const std::string& line,
				       bool& hadContinuation);

 protected:
  void extractLine(const std::string& line, bool setType);
  void decideType(const std::string& line);
  
 private:
  std::string text_;
  Type lineType_;
};

} // namespace cpsh

#endif
