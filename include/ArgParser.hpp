#pragma once

#include "ArgOption.hpp"
#include "TextProcessing.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
class ArgParser {
public:
  ArgParser(const std::string &programName = "",
            const std::string &description = "",
            const TextProcessing &textProcessing = {});
  void addPositionalArgument(const std::string &name,
                             const std::string &description = "");
  bool addOption(const ArgOption &option);
  bool addHelpOption();
  bool addVersionOption();
  void showHelp(std::ostream &out = std::cout) const;
  void parse(int argc, char *argv[]);

  std::string programName;
  std::string description;
  TextProcessing textProcessing;

private:
  std::vector<std::string> makeArgsText(const std::vector<ArgOption> &args,
                                        size_t &longestLeft) const;
  std::string makeSectionText(const std::vector<std::string> &argsText,
                              const size_t &longestLeft,
                              const std::vector<ArgOption> &args) const;
  std::vector<ArgOption> _positionals{};
  bool _hasHelp{false};
  std::string _hasVersion{""};
  std::vector<ArgOption> _options{};
  std::unordered_map<std::string, size_t> _optionPos{};
  std::unordered_map<size_t, std::vector<std::string>> _values{};
};