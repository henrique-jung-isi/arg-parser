#include "ArgParser.hpp"
#include <algorithm>
#include <sstream>

ArgParser::ArgParser(const std::string &programName,
                     const std::string &description,
                     const TextProcessing &textProcessing)
    : programName{programName}, description{description},
      textProcessing{textProcessing} {}

void ArgParser::addPositionalArgument(const std::string &name,
                                      const std::string &description) {
  _positionals.emplace_back(name, description);
}

bool ArgParser::addOption(const ArgOption &option) { //
  const auto &entries = option.arguments();
  if (entries.empty()) {
    return false;
  }

  for (const auto &entry : entries) {
    const auto it = _optionPos.find(entry);
    if (it != _optionPos.end()) {
      return false;
    }
  }
  const auto position = _optionPos.size();
  _options.push_back(option);
  for (const auto &entry : entries) {
    _optionPos.insert({entry, position});
  }

  return true;
}

bool ArgParser::addHelpOption() {
  return addOption({{"-h", "--help"}, "Show help."});
}

bool ArgParser::addVersionOption() {
  return addOption({{"-v", "--version"}, "Show version."});
}

void ArgParser::showHelp(std::ostream &out) const {
  if (!programName.empty()) {
    out << "Usage: " << programName;
    if (!_options.empty()) {
      out << " [Options]";
    }
    for (const auto &positional : _positionals) {
      out << " " << positional.arguments().front();
    }
    out << std::endl;
  }
  if (!description.empty()) {
    out << description << std::endl << std::endl;
  }
  size_t longestLeft = 0;
  const auto &allPositionals = makeArgsText(_positionals, longestLeft);
  const auto &allOptions = makeArgsText(_options, longestLeft);

  const auto &positionalsText =
      makeSectionText(allPositionals, longestLeft, _positionals);
  const auto &optionsText = makeSectionText(allOptions, longestLeft, _options);

  if (!_positionals.empty()) {
    out << "Positional arguments:" << std::endl;
    out << positionalsText << std::endl;
  }
  if (!_options.empty()) {
    out << "Options:" << std::endl;
    out << optionsText << std::endl;
  }
}

void ArgParser::parse(int argc, char *argv[]) {
  for (auto i = 0; i < argc; i++) {
    const auto argument = argv[i];
  }
}

std::vector<std::string>
ArgParser::makeArgsText(const std::vector<ArgOption> &args,
                        size_t &longestLeft) const {
  std::vector<std::string> all;
  for (const auto &arg : args) {
    std::string text = arg.argumentsDisplayText();
    all.push_back(text);
    longestLeft = std::max(longestLeft, text.size());
  }
  return all;
}

std::string
ArgParser::makeSectionText(const std::vector<std::string> &argsText,
                           const size_t &longestLeft,
                           const std::vector<ArgOption> &args) const {
  auto it = argsText.begin();
  std::string text;
  for (const auto &arg : args) {
    text += textProcessing.wrapText(*it, longestLeft, arg.description());
    it++;
  }
  return text;
}
