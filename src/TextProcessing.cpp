#include "TextProcessing.hpp"
#include <cassert>

std::string TextProcessing::sectionFrom(const std::string &target,
                                        std::size_t &start,
                                        const std::size_t &count) {
  const auto &section = target.substr(start, count);
  start += section.size();
  return section;
}

std::string TextProcessing::wrapText(const std::string &option,
                                     size_t maximumWidth,
                                     const std::string &description) const {
  maximumWidth = std::min(
      maximumLeftLength - spacing.size() - indentation.size(), maximumWidth);
  size_t optionIndex = 0;
  std::string text;
  size_t lineStart = 0;
  int lastBreakable = -1;
  const auto &max = rightColumnSize(maximumWidth);
  size_t current = 0;
  const auto total = description.size();

  for (size_t i = 0; i < total; i++) {
    current++;
    const auto &c = description.at(i);
    if (::isspace(c)) {
      lastBreakable = i;
    }

    int breakAt = -1;
    int nextLineStart = -1;
    if (current > max && lastBreakable != -1) {
      breakAt = lastBreakable;
      nextLineStart = lastBreakable + 1;
    } else if ((current >= max && lastBreakable == -1) || i == total - 1) {
      breakAt = i + 1;
      nextLineStart = breakAt;
    } else if (c == '\n') {
      breakAt = i;
      nextLineStart = i + 1;
    }

    if (breakAt != -1) {
      const auto name = sectionFrom(option, optionIndex, maximumWidth);
      const auto leftColumn = indentation + name + spacing;

      const auto count = breakAt - lineStart;
      const auto rightColumn = description.substr(lineStart, count);
      assert(rightColumn.size() <= max);

      const auto emptySpace = maximumWidth - name.size();
      const std::string emptyString(emptySpace, ' ');
      assert((leftColumn + emptyString + rightColumn).size() <= lineLength);

      text += leftColumn + emptyString + rightColumn + '\n';
      current = 1;
      lastBreakable = -1;
      lineStart = nextLineStart;
      if (lineStart < total && ::isspace(description.at(lineStart))) {
        lineStart++; // don't start a line with a space
      }
      i = lineStart;
    }
  }

  while (optionIndex < option.size()) {
    text += indentation + sectionFrom(option, optionIndex, maximumWidth) + '\n';
  }
  return text;
}

size_t TextProcessing::rightColumnSize(const size_t &maximumWidth) const {
  return lineLength - indentation.size() - maximumWidth - spacing.size();
}
