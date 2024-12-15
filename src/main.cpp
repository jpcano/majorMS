#include <utf8.h>
#include <utf8/checked.h>

#include <cxxopts.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "dictionary.h"

int main(int argc, char** argv) {
  cxxopts::Options options("majorMS", "Major Mnemonic System");

  options.add_options()("v,version", "Show version")("h,help", "Print usage")(
      "numbers", "The number", cxxopts::value<std::vector<std::string>>())(
      "csv", "Save to CSV file", cxxopts::value<std::string>());

  options.parse_positional({"numbers"});

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }

  if (result.count("version")) {
    std::cout << "Development version" << std::endl;
    exit(0);
  }

  auto numbers = result["numbers"].as<std::vector<std::string>>();
  if (numbers.size() != 1 && numbers.size() != 2) {
    std::cout << "You should provide one or two numbers" << std::endl;
    exit(0);
  } else if (numbers.size() == 1) {
    numbers.push_back(numbers[0]);
  }

  Dictionary dictionary("../data/config.json", "../data/es_ES.txt");

  if (result.count("csv")) {
    dictionary.saveWords(result["csv"].as<std::string>(), numbers[0],
                         numbers[1]);
    exit(0);
  }

  for (int n = stoi(numbers[0]); n <= stoi(numbers[1]); n++) {
    std::cout << std::to_string(n) << ": ";
    auto words = dictionary.getWords(std::to_string(n));
    if (words.size() > 0) {
      int i = 0;
      for (auto& word : words) {
        std::cout << word.name << " (" << word.ipa << ")";
        if (i < words.size() - 1) {
          std::cout << ", ";
        }
        i++;
      }
    }
    if (n == stoi(numbers[1]))
      std::cout << std::endl;
    else
      std::cout << std::endl << std::endl;
  }
  return 0;
}
