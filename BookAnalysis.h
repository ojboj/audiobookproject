#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <Python.h> // To run Machine Learning code
#include "Chapter.h"
#include "Character.h"
#include "StringHelper.h"

using namespace std;

class BookAnalysis {
  public:
    // Function to return a sorted (by amount of dialogue) vector of characters in the book.
    static vector<Character> get_characters(vector<Chapter> chapters) {
      vector<Character> characters;
      // Get capitalised words
      vector<string> capitals = get_capitals(chapters);
      // Output into a .txt file for python
      output_capitals(capitals);
      // Use python and keras to classify the words

      // Use output from ^ to get only the words that are thought to be proper nouns

      // Delete temporay files

      // Get dialogue associated with character names

      return(characters);
    }

  private:
    // Function to get all the names of characters in the book.
    static vector<string> get_capitals(vector<Chapter> chapters) {
      vector<string> capitalised;
      // Get a vector of each capitalised word in the book
      // For each chapter
      for (int i = 0; i < chapters.size(); i++) {
        vector<string> words = StringHelper::get_words(chapters[i].body);
        // For each word in chapter.body
        for (int j = 0; j < words.size(); j++) {
          if (StringHelper::is_capitalised(words[j])) {
            capitalised.push_back(words[j]);
          }
        }
      }
      return(capitalised);
    }

    // Function to output list of capital letters to a .txt file for python.
    static void output_capitals(vector<string> capitals) {
      string out_file_path = "./capitalised_words.txt";
      // Opening file to write to
      ofstream out_file;
      out_file.open(out_file_path);
      // For each capitalised word
      for (int i = 0; i < capitals.size(); i++) {
        out_file << capitals[i] << endl;
      }
      // Close out stream
      out_file.close();
    }
};
