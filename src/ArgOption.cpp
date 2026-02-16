#include "ArgOption.hpp"
#include "TextProcessing.hpp"
#include <ostream>
#include <sstream>
ArgOption::ArgOption(const std::string &argument,
                     const std::string &description,
                     const std::string &valueName,
                     const std::vector<std::string> &defaultValues)
    : ArgOption{{argument}, description, valueName, defaultValues} {}

ArgOption::ArgOption(const std::vector<std::string> &arguments,
                     const std::string &description,
                     const std::string &valueName,
                     const std::vector<std::string> &defaultValues)
    : _arguments{arguments}, _description{description}, _valueName{valueName},
      _defaultValues{defaultValues} {}

ArgOption::ArgOption(const std::initializer_list<std::string> &arguments,
                     const std::string &description,
                     const std::string &valueName,
                     const std::vector<std::string> &defaultValues)
    : ArgOption{std::vector<std::string>{arguments},
                description,
                valueName,
                defaultValues} {}

const std::vector<std::string> &ArgOption::arguments() const {
  return _arguments;
}

const std::string &ArgOption::description() const { return _description; }

const std::string &ArgOption::valueName() const { return _valueName; }

const std::vector<std::string> &ArgOption::defaultValues() const {
  return _defaultValues;
}

std::ostream &operator<<(std::ostream &os, const ArgOption &option) {
  os << "ArgOption(arguments: " << option._arguments;
  os << ", description: \"" << option._description + "\"";
  os << ", valueName: \"" << option._valueName + "\"";
  os << ", defaultValue: " << option._defaultValues << ")";
  return os;
}
