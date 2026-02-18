#include <algorithm>
#include <arg_parser/ArgParser.hpp>
#include <exception>
#include <sstream>

ArgParser::ArgParser(const ParserConfig &config) : config{config} {}

void ArgParser::addPositionalArgument(const std::string &name,
                                      const std::string &description) {
  _positionalOptions.emplace_back(name, description);
}

bool ArgParser::addOption(const ArgOption &option) {
  const auto &entries = option.arguments();
  if (entries.empty()) {
    return false;
  }

  for (const auto &entry : entries) {
    const auto it = _optionPosition.find(entry);
    if (it != _optionPosition.end()) {
      return false;
    }
  }
  const auto position = _options.size();
  _options.push_back(option);
  for (const auto &entry : entries) {
    _optionPosition.insert({entry, position});
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
  if (!config.programName.empty()) {
    out << "Usage: " << config.programName;
    if (!_options.empty()) {
      out << " [Options]";
    }
    for (const auto &positional : _positionalOptions) {
      out << " " << positional.arguments().front();
    }
    out << std::endl;
  }
  if (!config.description.empty()) {
    out << config.description << std::endl << std::endl;
  }
  size_t longestLeft = 0;
  const auto &allPositionals = makeArgsText(_positionalOptions, longestLeft);
  const auto &allOptions = makeArgsText(_options, longestLeft);

  const auto &positionalsText =
      makeSectionText(allPositionals, longestLeft, _positionalOptions);
  const auto &optionsText = makeSectionText(allOptions, longestLeft, _options);

  if (!_positionalOptions.empty()) {
    out << "Positional arguments:" << std::endl;
    out << positionalsText << std::endl;
  }
  if (!_options.empty()) {
    out << "Options:" << std::endl;
    out << optionsText << std::endl;
  }
}

void ArgParser::showVersion(std::ostream &out) const {
  out << config.programName << " " << config.version << "\n";
}

void ArgParser::parse(int argc, char *argv[]) {
  _parsedOptions.clear();
  _parsedPositionals.clear();
  _uknownArguments.clear();

  std::vector<std::string> arguments(argv, argv + argc);
  auto it = arguments.cbegin();

  if (config.programName.empty()) config.programName = *it;
  it++;

  for (; it != arguments.cend(); it++) {
    const auto &argument = *it;
    if (argument.compare("--") == 0) {
      // TODO: store remaining arguments as unparsed
    } else if (argument.starts_with('-')) {
      parseOption(it, arguments.cend());
    } else {
      _parsedPositionals.push_back(argument);
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

  if (!_uknownArguments.empty()) {
    std::cerr << config.unknownOptionMessage << _uknownArguments << std::endl;
    showHelp();
    exit(1);
  }
}

bool ArgParser::isSet(const std::string &option) const {
  const auto position = _optionPosition.find(option);
  if (position == _optionPosition.cend()) {
    return false;
  }
  return _parsedOptions.contains(position->second);
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
  const auto position = _optionPosition.find(option);
  if (position == _optionPosition.cend()) {
    throw std::invalid_argument("Uknown argument: " + option);
  }
  if (!_parsedOptions.contains(position->second)) {
    const auto &argOption = _options[position->second];
    return argOption.defaultValues();
  }
  return _parsedOptions.at(position->second);
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
  return _parsedPositionals;
}

const std::vector<ArgOption> &ArgParser::availableOptions() const {
  return _options;
}

std::vector<std::string>
ArgParser::makeArgsText(const std::vector<ArgOption> &args,
                        size_t &longestLeft) const {
  std::vector<std::string> all;
  for (const auto &arg : args) {
    std::string text = optionDisplayText(arg);
    all.push_back(text);
    longestLeft = std::max(longestLeft, text.size());
  }
  return all;
}

std::string ArgParser::optionDisplayText(const ArgOption &option) const {
  std::string text;
  for (char comma[]{'\0', ' ', '\0'}; const auto &arg : option.arguments()) {
    text += comma + arg;
    comma[0] = ',';
  }

  if (!option.valueName().empty()) {
    text += " " + config.textProcessing.leftValueSymbol + option.valueName() +
            config.textProcessing.rightValueSymbol;
  }
  return text;
}

std::string
ArgParser::makeSectionText(const std::vector<std::string> &argsText,
                           const size_t &longestLeft,
                           const std::vector<ArgOption> &args) const {
  auto it = argsText.begin();
  std::string text;
  for (const auto &arg : args) {
    text += config.textProcessing.wrapText(*it, longestLeft, arg.description());
    it++;
  }
  return text;
}

bool ArgParser::addParsedOption(const std::string &option) {
  // TODO: option to parse arguments in the form --arg=value
  auto it = _optionPosition.find(option);
  if (it == _optionPosition.end()) {
    _uknownArguments.push_back(option);
    return false;
  }
  const auto &position = it->second;
  _parsedOptions.insert({position, {}});
  return true;
}

void ArgParser::parseOption(std::vector<std::string>::const_iterator &it,
                            std::vector<std::string>::const_iterator end) {
  // TODO: unkown argument passed
  if (!addParsedOption(*it)) return;

  const auto &position = _optionPosition[*it];
  const auto &option = _options[position];
  const auto hasValue = !option.valueName().empty();
  if (hasValue && it + 1 != end) {
    it++;
    _parsedOptions[position].push_back(*it);
  } else {
    // TODO: value not passed with option that requires it
  }
}