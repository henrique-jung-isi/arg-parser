#pragma once
#include <ostream>
#include <string>
#include <vector>

class ArgOption {
public:
  ArgOption(const std::string &argument, const std::string &description,
            const std::string &valueName = "",
            const std::string &defaultValue = "");
  ArgOption(const std::initializer_list<std::string> &arguments,
            const std::string &description, const std::string &valueName = "",
            const std::string &defaultValue = "");
  ArgOption(const std::vector<std::string> &arguments,
            const std::string &description, const std::string &valueName = "",
            const std::string &defaultValue = "");

  const std::vector<std::string> &arguments() const;
  std::string argumentsDisplayText() const;
  const std::string &description() const;
  const std::string &valueName() const;
  const std::string &defaultValue() const;

  friend std::ostream &operator<<(std::ostream &os, const ArgOption &option);

  std::string leftValueSymbol = "<";
  std::string rightValueSymbol = ">";

private:
  std::vector<std::string> _arguments;
  std::string _description;
  std::string _valueName;
  std::string _defaultValue;
};