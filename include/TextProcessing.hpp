#pragma once

#include <string>
class TextProcessing {
public:
  static std::string sectionFrom(const std::string &target, std::size_t &start,
                                 const std::size_t &count);

  // TODO: enable option for UTF32 parsing - one character always sits in one
  // position of the array
  std::string wrapText(const std::string &option, std::size_t maximumWidth,
                       const std::string &description) const;
  size_t rightColumnSize(const std::size_t &maximumWidth) const;

  std::string indentation{"  "};
  std::string spacing{"  "};
  std::size_t lineLength{80};
  std::size_t maximumLeftLength{lineLength / 2};
};