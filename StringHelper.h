#pragma once

#include <iostream>
#include <string>
#include <cstring>

using namespace std;

class StringHelper {
  public:
    // Function to return a vecor of words from a string.
    static vector<string> get_words(string full_string) {
      vector<string> words;
      string current_word = "";
      // For each char in full_string
      for (int i = 0; i < full_string.size(); i++) {
        // If not space, construct current word
        if (!(full_string[i] == ' ')) {
          current_word = current_word + full_string[i];
        }
        // Else, add word to vector and reset temp variable
        else {
          words.push_back(current_word);
          current_word = "";
        }
      }
      return(words);
    }

    // Function to check if a string is capitalised.
    static bool is_capitalised(string full_string) {
      vector<char> capital_letters = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
      'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
      'X', 'Y', 'Z'};
      // Check against each capital letter
      for (int i = 0; i < capital_letters.size(); i++) {
        if (full_string[0] == capital_letters[i]) {
          return(true);
        }
      }
      return(false);
    }

    // Function to check if a string starts with a prefix.
    static bool starts_with(string full_string, string prefix) {
      // If input isn't empty
      if (!full_string.empty() && !prefix.empty()) {
        // Find the substring of the full_string to the same size as the prefix
        string prefix_of_full_string = full_string.substr(0, prefix.size());
        // Compare
        return(prefix_of_full_string.compare(prefix) == 0);
      }
      else {
        return(false);
      }
    }

    // Function to check if a string ends with a suffix.
    static bool ends_with(string full_string, string suffix) {
      // If input isn't empty
      if (!full_string.empty() && !suffix.empty()) {
        // Find the substring of the full_string to the same size as the suffix
        string suffix_of_full_string = full_string.substr(full_string.size()-suffix.size(), suffix.size());
        // Compare
        return(suffix_of_full_string.compare(suffix) == 0);
      }
      else {
        return(false);
      }
    }

    // Function to check if a string containts another string.
    static bool contains(string full_string, string candidate) {
      int successing_chars = 0;
      // Check if strings are empty and candidate is smaller
      if (!full_string.empty() && !candidate.empty() && candidate.size() <= full_string.size()) {
        // Iterate through chars
        for (int i = 0; i < full_string.size(); i++) {
          // If find the first char
          if (full_string[i] == candidate[0]) {
            // Increment running total
            successing_chars ++;
            for (int j = 1; j < candidate.size(); j++) {
              // If not a match, reset and break this test
              if (!(full_string[i+j] == candidate[j])) {
                successing_chars = 0;
                break;
              }
              // If a match, increment
              else {
                successing_chars ++;
                // If found all successing chars, return true
                if (successing_chars == candidate.size()) {
                  return(true);
                }
              }
            }
          }
        }
        return(false);
      }
      else {
        return(false);
      }
    }
};
