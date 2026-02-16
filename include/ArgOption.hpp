#pragma once
#include <ostream>
#include <string>
#include <vector>

class ArgOption {
public:
  ArgOption(const std::string &argument, const std::string &description,
            const std::string &valueName = "",
            const std::vector<std::string> &defaultValues = {});
  ArgOption(const std::initializer_list<std::string> &arguments,
            const std::string &description, const std::string &valueName = "",
            const std::vector<std::string> &defaultValues = {});
  ArgOption(const std::vector<std::string> &arguments,
            const std::string &description, const std::string &valueName = "",
            const std::vector<std::string> &defaultValues = {});

  const std::vector<std::string> &arguments() const;
  const std::string &description() const;
  const std::string &valueName() const;
  const std::vector<std::string> &defaultValues() const;

  friend std::ostream &operator<<(std::ostream &os, const ArgOption &option);

private:
  std::vector<std::string> _arguments;
  std::string _description;
  std::string _valueName;
  std::vector<std::string> _defaultValues;
};