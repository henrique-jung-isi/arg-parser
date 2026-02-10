#pragma once

#include "ArgOption.hpp"
#include "TextProcessing.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
class ArgParser {
public:
  ArgParser(const TextProcessing &textProcessing = {});

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

  const std::vector<std::string> &positionalValues() const;
  const std::unordered_map<size_t, std::vector<std::string>> &values() const;
  const std::vector<ArgOption> &availableOptions() const;

  template <class T> T typedValue(const ArgOption &option) const {
    return typedValue<T>(option.arguments().front());
  }

  template <class T> T typedValue(const std::string &option) const;

  std::string programName{};
  std::string description{};
  std::string version{"1.0"};
  std::string uknownOptionMessage{"Unkown option(s): "};
  TextProcessing textProcessing{};

private:
  std::vector<std::string> makeArgsText(const std::vector<ArgOption> &args,
                                        size_t &longestLeft) const;
  std::string makeSectionText(const std::vector<std::string> &argsText,
                              const size_t &longestLeft,
                              const std::vector<ArgOption> &args) const;
  bool addParsedOption(const std::string &option);
  void parseOption(std::vector<std::string>::const_iterator &it,
                   std::vector<std::string>::const_iterator end);

  std::vector<ArgOption> _positionals{};
  bool _hasHelp{false};
  bool _hasVersion{false};
  std::vector<ArgOption> _options{};
  std::unordered_map<std::string, size_t> _optionPos{};
  std::unordered_map<size_t, std::vector<std::string>> _values{};
  std::vector<std::string> _positionalValues{};
  std::vector<std::string> _uknownValues{};
};