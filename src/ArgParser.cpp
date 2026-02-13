#include "ArgParser.hpp"
#include <algorithm>
#include <exception>
#include <sstream>

ArgParser::ArgParser(const TextProcessing &textProcessing)
    : textProcessing{textProcessing} {}

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
  const auto position = _options.size();
  _options.push_back(option);
  for (const auto &entry : entries) {
    _optionPos.insert({entry, position});
  }

  return true;
}

bool ArgParser::addHelpOption() {
  _hasHelp = true;
  return addOption({{"-h", "--help"}, "Show help."});
}

bool ArgParser::addVersionOption() {
  _hasVersion = true;
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

void ArgParser::showVersion(std::ostream &out) const {
  out << programName << " " << version << "\n";
}

void ArgParser::parse(int argc, char *argv[]) {
  _values.clear();
  _positionalValues.clear();
  _uknownValues.clear();

  std::vector<std::string> arguments(argv, argv + argc);
  auto it = arguments.cbegin();

  if (programName.empty()) programName = *it;
  it++;

  for (; it != arguments.cend(); it++) {
    const auto &argument = *it;
    if (argument.compare("--") == 0) {
      // TODO: store remaining arguments as unparsed
    } else if (argument.starts_with('-')) {
      parseOption(it, arguments.cend());
    } else {
      _positionalValues.push_back(argument);
    }
  }

  if (_hasHelp && isSet("-h")) {
    showHelp();
    exit(0);
  }

  if (_hasVersion && isSet("-v")) {
    showVersion();
    exit(0);
  }

  if (!_uknownValues.empty()) {
    std::cerr << uknownOptionMessage << _uknownValues << std::endl;
    showHelp();
    exit(1);
  }
}

bool ArgParser::isSet(const std::string &option) const { //
  const auto position = _optionPos.find(option);
  if (position == _optionPos.cend()) {
    return false;
  }
  return _values.contains(position->second);
}

bool ArgParser::isSet(const ArgOption &option) const {
  for (const auto &arg : option.arguments()) {
    if (isSet(arg)) {
      return true;
    }
  }
  return false;
}

const std::vector<std::string> &
ArgParser::values(const std::string &option) const {
  const auto position = _optionPos.find(option);
  if (position == _optionPos.cend()) {
    throw std::invalid_argument("Uknown argument: " + option);
  }
  if (!_values.contains(position->second)) {
    const auto &argOption = _options[position->second];
    return argOption.defaultValues();
  }
  return _values.at(position->second);
}

const std::vector<std::string> &
ArgParser::values(const ArgOption &option) const {
  return values(option.arguments().front());
}

const std::string &ArgParser::value(const std::string &option) const {
  return values(option).front();
}

const std::string &ArgParser::value(const ArgOption &option) const {
  return values(option.arguments().front()).front();
}

const std::vector<std::string> &ArgParser::positionalValues() const {
  return _positionalValues;
}

const std::vector<ArgOption> &ArgParser::availableOptions() const {
  return _options;
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

bool ArgParser::addParsedOption(const std::string &option) { //
  auto it = _optionPos.find(option);
  if (it == _optionPos.end()) {
    _uknownValues.push_back(option);
    return false;
  }
  const auto &position = it->second;
  _values.insert({position, {}});
  return true;
}

void ArgParser::parseOption(std::vector<std::string>::const_iterator &it,
                            std::vector<std::string>::const_iterator end) {
  if (!addParsedOption(*it)) return; // TODO: unkown argument passed

  const auto &position = _optionPos[*it];
  const auto &option = _options[position];
  const auto hasValue = !option.valueName().empty();
  if (hasValue && it + 1 != end) {
    it++;
    _values[position].push_back(*it);
  } else {
    // TODO: value not passed with option that requires it
  }
}