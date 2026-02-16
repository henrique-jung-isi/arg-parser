#include <arg_parser/ArgParser.hpp>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
  TextProcessing text{.indentation{"  "},
                      .spacing{"  "},
                      .lineLength{80},
                      .maximumLeftLength{35}};
  ArgParser parser{{.programName = "ArgParser-example",
                    .description = "This is the description for the program.",
                    .textProcessing = text}};

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
  parser.showHelp();

  cout << "Debug:\n";
  cout << "Positionals: \n" << parser.positionalValues() << "\n";
  const auto options = parser.availableOptions();

  cout << anArg.arguments() << ": " << parser.value(anArg) << "\n";
  cout << "\n";

  cout << sizeOption.arguments() << ": " << parser.value(sizeOption) << "\n";
  cout << "string: " << parser.value<string>(sizeOption) << "\n";
  cout << "int: " << parser.value<int>(sizeOption) << "\n";
  cout << "double: " << parser.value<double>(sizeOption) << "\n";

  return 0;
}
