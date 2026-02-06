#include <arg_parser/ArgParser.hpp>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  ArgParser parser("ArgParser-example", "This is description for the program.");
  parser.addPositionalArgument("INPUT",
                               "Some positional input for the example");
  parser.addPositionalArgument("OUTPUT",
                               "Some positional output for the example");
  parser.addHelpOption();
  parser.addVersionOption();
  ArgOption anArg({"-f", "--file"}, "Some argument that expects input", "file");
  parser.addOption(anArg);
  ArgOption veryLong(
      {"-l", "--longer"},
      "This is an option to represent really long and verbose descriptions, it "
      "serves as an example and test for wrapText from TextProcessing.");
  parser.addOption(veryLong);
  ArgOption noSpaces({"-L", "--longer2"},
                     "WhatHappensIfThereAreNoBrakesForTheDescription?"
                     "WrapTextShouldJustWrapWhenTheLimitIsReached");
  parser.addOption(noSpaces);

  parser.showHelp();

  return 0;
}
