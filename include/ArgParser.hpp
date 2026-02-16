#pragma once

#include "ArgOption.hpp"
#include "TextProcessing.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

template <class T>
concept Number = std::is_arithmetic_v<T>;
template <class T>
concept String = std::is_same_v<T, std::string>;
template <class T>
concept Convertible = Number<T> || String<T>;

struct ParserConfig {
  std::string programName{""};
  std::string description{""};
  std::string version{"1.0"};
  std::string unknownOptionMessage{"Unkown option(s): "};
  TextProcessing textProcessing{};
};

class ArgParser {
public:
  ArgParser(const ParserConfig &config = {});

  void addPositionalArgument(const std::string &name,
                             const std::string &description = "");
  bool addOption(const ArgOption &option);
  bool addHelpOption();
  bool addVersionOption();
  void showHelp(std::ostream &out = std::cout) const;
  void showVersion(std::ostream &out = std::cout) const;
  void parse(int argc, char *argv[]);

  bool isSet(const std::string &option) const;
  bool isSet(const ArgOption &option) const;

  const std::vector<std::string> &values(const std::string &option) const;
  const std::vector<std::string> &values(const ArgOption &option) const;

  const std::string &value(const std::string &option) const;
  const std::string &value(const ArgOption &option) const;

  template <String T> T value(const std::string &option) const;
  template <Number T> T value(const std::string &option) const;

  template <Convertible T> T value(const ArgOption &option) const;

  const std::vector<std::string> &positionalValues() const;
  const std::vector<ArgOption> &availableOptions() const;

  ParserConfig config;

private:
  std::vector<std::string> makeArgsText(const std::vector<ArgOption> &args,
                                        size_t &longestLeft) const;

  std::string optionDisplayText(const ArgOption &option) const;
  std::string makeSectionText(const std::vector<std::string> &argsText,
                              const size_t &longestLeft,
                              const std::vector<ArgOption> &args) const;
  bool addParsedOption(const std::string &option);
  void parseOption(std::vector<std::string>::const_iterator &it,
                   std::vector<std::string>::const_iterator end);

  bool _hasHelp{false};
  bool _hasVersion{false};
  std::vector<ArgOption> _positionalOptions{};
  std::vector<ArgOption> _options{};
  std::unordered_map<std::string, size_t> _optionPosition{};
  std::unordered_map<size_t, std::vector<std::string>> _parsedOptions{};
  std::vector<std::string> _parsedPositionals{};
  std::vector<std::string> _uknownArguments{};
};

template <Convertible T> T ArgParser::value(const ArgOption &option) const {
  return value<T>(option.arguments().front());
}

template <Number T> T ArgParser::value(const std::string &option) const {
  std::istringstream ss(value(option));
  T converted;
  ss >> converted;
  return converted;
}

template <> std::string ArgParser::value(const std::string &option) const {
  return value(option);
}