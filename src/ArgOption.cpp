#include "ArgOption.hpp"
#include <ostream>
#include <sstream>
ArgOption::ArgOption(const std::string &argument,
                     const std::string &description,
                     const std::string &valueName,
                     const std::string &defaultValue)
    : ArgOption{{argument}, description, valueName, defaultValue} {}

ArgOption::ArgOption(const std::vector<std::string> &arguments,
                     const std::string &description,
                     const std::string &valueName,
                     const std::string &defaultValue)
    : _arguments{arguments}, _description{description}, _valueName{valueName},
      _defaultValue{defaultValue} {}

ArgOption::ArgOption(const std::initializer_list<std::string> &arguments,
                     const std::string &description,
                     const std::string &valueName,
                     const std::string &defaultValue)
    : ArgOption{std::vector<std::string>{arguments},
                description,
                valueName,
                defaultValue} {}

const std::vector<std::string> &ArgOption::arguments() const {
  return _arguments;
}

std::string ArgOption::argumentsDisplayText() const {
  std::string text;
  for (const auto &arg : _arguments) {
    text += arg + ", ";
  }
  if (!text.empty()) {
    text.erase(text.size() - 2);
  }
  if (!_valueName.empty()) {
    text += " " + leftValueSymbol + _valueName + rightValueSymbol;
  }
  return text;
}

const std::string &ArgOption::description() const { return _description; }

const std::string &ArgOption::valueName() const { return _valueName; }

const std::string &ArgOption::defaultValue() const { return _defaultValue; }

std::ostream &operator<<(std::ostream &os, const ArgOption &option) {
  std::string text = "ArgOption(arguments: [";
  for (const auto &arg : option._arguments) {
    text += "\"" + arg + "\", ";
  }
  if (!option._arguments.empty()) {
    text.erase(text.size() - 2);
  }
  text += "], description: \"" + option._description + "\"";
  text += ", valueName: \"" + option._valueName + "\"";
  text += ", defaultValue: \"" + option._defaultValue + "\")";
  os << text;
  return os;
}
