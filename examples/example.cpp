#include <arg_parser/ArgParser.hpp>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  ArgParser parser;
  parser.programName = "ArgParser-example";
  parser.description = "This is the description for the program.";

  parser.addPositionalArgument("INPUT",
                               "Some positional input for the example");
  parser.addPositionalArgument("OUTPUT",
                               "Some positional output for the example");
  parser.addHelpOption();
  parser.addVersionOption();
  ArgOption anArg(
      {"-f", "--file"}, "Some argument that expects input", "file", {"."});
  parser.addOption(anArg);
  ArgOption veryLong(
      {"-l", "--longer"},
      "This is an option to represent really long and verbose descriptions, it "
      "serves as an example and test for wrapText from TextProcessing.");
  parser.addOption(veryLong);
  ArgOption noSpaces({"-L", "--longer2"},
                     "WhatHappensIfThereAreNoBrakesForTheDescription?"
                     "WrapTextShouldJustWrapWhenTheLimitIsReached."
                     "ACharacterIsASpaceIfTheFunction\"::isspace(int)"
                     "\"From\"cctype\"ReturnsTrue.");
  parser.addOption(noSpaces);
  ArgOption sizeOption({"-s", "--size"},
                       "Variable to test template method to convert type.",
                       "size",
                       {"0"});
  parser.addOption(sizeOption);

  parser.parse(argc, argv);

  cout << "Debug:\n";
  cout << "Positionals: \n" << parser.positionalValues() << "\n";
  const auto values = parser.values();
  const auto options = parser.availableOptions();
  for (const auto &[pos, strings] : values) {
    cout << options[pos].arguments() << ": " << strings << "\n";
  }
  cout << anArg.arguments() << ": " << parser.value(anArg) << "\n";
  cout << "\n";

  cout << sizeOption.arguments() << ": " << parser.value(sizeOption) << "\n";
  cout << "string: " << parser.typedValue<std::string>(sizeOption) << "\n";
  cout << "int: " << parser.typedValue<int>(sizeOption) << "\n";
  return 0;
}
