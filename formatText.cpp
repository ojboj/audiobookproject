#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>
#include <vector>
#include <zip.h>
#include "Chapter.h"

using namespace std;

// -- Helper functions.

// Delcarations.
vector<Chapter> chapterise(string);
string remove_formatting(string);
bool contains(string, string);
void unzip_book(string);
string get_filename_only(string);
string remove_ext(string);
bool starts_with(string, string);
bool ends_with(string, string);

// Function to turn folder of .txt files into chapter objects.
vector<Chapter> chapterise(string book_name) {
  vector<Chapter> chapters;
  int count_of_chapters = 0;

  // Iterate through all files in the folder
  for(auto& p: filesystem::directory_iterator(book_name)) {
    // If .txt file
    if (ends_with(p.path().string(), ".txt")) {
      // Load text from the found text file
      vector<string> file_buffer;
      ifstream in_file;
      string temp_line;
      in_file.open("./" + p.path().string());
      if (in_file.is_open()) {
        while(getline(in_file, temp_line)) {
          file_buffer.push_back(temp_line);
        }
      }
      else {
        cerr << " Failed to open: " << p.path() << endl;
      }
      in_file.close();

      // Set initial chapter values
      bool is_chapter = false;
      int chapter_number = 0;
      string chapter_title = "";
      string disclaimer = "";
      string body;

      // Check file_buffer data to see if chapter
      for (int i = 0; i < file_buffer.size(); i++) {
        // Line signifying a chapter is unique at this point
        if (starts_with(file_buffer[i], "<h2 style")) {
          // We have a chapter bois
          is_chapter = true;

          // Get chapter number
          chapter_number = count_of_chapters;
          count_of_chapters ++;

          // Get chapter title
          int title_start = 0;
          int title_end = 0;
          for (int j = 0; j < file_buffer[i].size(); j++) {
            if (file_buffer[i][j-1] == '\"' && file_buffer[i][j] == '>') {
              title_start = j+1;
            }
            else if (file_buffer[i][j] == '<' && file_buffer[i][j+1] == '/') {
              title_end = j;
            }
          }
          chapter_title = file_buffer[i].substr(title_start, title_end-title_start);
        }
        // Check for line leading paragraphs
        else if (is_chapter && starts_with(file_buffer[i], "<p>")) {
          // Find if body or disclaimer

          // If disclaimer
          if (contains(file_buffer[i], "Disclaimer") || contains(file_buffer[i], "disclaimer")) {
            // Get disclaimer paragraph
            string raw_disclaimer = "";
            for (int j = i; j < file_buffer.size(); j++) {
              // Check if line is still part of disclaimer
              if (!contains(file_buffer[j], "<hr/>")) {
                raw_disclaimer = raw_disclaimer + file_buffer[j];
              }
              // If line is where chapter body begins, set i and break
              else {
                i = j;
                break;
              }
            }
            // Format raw disclaimer text nicely, remove formatting symbolica
            disclaimer = remove_formatting(raw_disclaimer);
          }
          // If body
          else {
            // Get all of body, replace </p> with \n
            string raw_body = "";
            for (int j = i; j < file_buffer.size(); j++) {
              // Check if line is still part of body
              if (!contains(file_buffer[j], "/div")) {
                raw_body = raw_body + file_buffer[j];
              }
              // If line is where chapter body ends, set i and break
              else {
                i = j;
                break;
              }
            }
            // Format raw body text nicely, remove formatting symbolica
            body = remove_formatting(raw_body);
          }
        }
      }

      // Info gathered for chapter, make the Chapter object
      chapters.push_back(Chapter(chapter_number, chapter_title, disclaimer, body));
    }
  }

  // Return vector of chapters
  return(chapters);
}

// Function to remove any formatting from text.
string remove_formatting(string raw_html) {
  string text = "";
  for (int i = 0; i < raw_html.size(); i++) {
    // If you find any html formatting
    if (raw_html[i] == '<') {
      bool new_line = false;
      // If find </p>, mark for replacement with \n
      if (raw_html[i+1] == '/' && raw_html[i+2] == 'p') {
        new_line = true;
      }
      // Find where it ends and being loop from there, skipping it
      int j = i+1;
      while (raw_html[j] != '>' && j < raw_html.size()) {
        j++;
      }
      // If marked for new line, add \n
      if (new_line) {
        text = text + '\n';
      }
      i = j;
    }
    else {
      text = text + raw_html[i];
    }
  }

  return(text);
}

// Function to check if a string containts another string.
bool contains(string full_string, string candidate) {
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

// Function to unzip an epub file.
void unzip_book(string book_filename) {
  // Create book_path c string
  char book_path[strlen("./") + strlen(book_filename.c_str()) +1];
  strcpy(book_path, "./");
  strcat(book_path, book_filename.c_str());

  // Create path to folder
  string book_name = remove_ext(get_filename_only(book_filename));
  string chapters_folder_path = "./" + book_name + "/";

  // Use filesystem to create a directory
  filesystem::path path = chapters_folder_path;
  filesystem::create_directory(path);

  // Open zip file
  int err = 0;
  zip *z = zip_open(book_path, 0, &err);

  // loop through chapters here
  int number_of_files = zip_get_num_entries(z, 0);
  int count_of_html_files = 0;

  for (int i = 0; i < number_of_files; i++) {
    // Get i-th file
    const char* name = zip_get_name(z, i, 0);

    // Test if .html
    string name_of_file = name;
    if (ends_with(name_of_file, ".html")) {
      // Get file info
      struct zip_stat st;
      zip_stat_init(&st);
      zip_stat(z, name, 0, &st);

      // Setup storage of file
      char* contents = new char[st.size];

      // Read file and store in contents
      zip_file *f = zip_fopen(z, name, 0);
      zip_fread(f, contents, st.size);
      zip_fclose(f);

      // Output file to folder
      string out_file_path = chapters_folder_path + "html_file" + to_string(count_of_html_files) + ".txt";
      if (!ofstream(out_file_path).write(contents, st.size))
      {
        cerr << "unzip_book: could not write file: " << name_of_file << endl;
      }
      count_of_html_files ++;
    }

  }

  zip_close(z);
}

// Function to get the filename only.
string get_filename_only(string file_path) {
  bool found_slash = false;
  int final_slash_index = 0;

  for (int i = 0; i < file_path.size(); i++) {
    if (file_path[i] == '/') {
      final_slash_index = i;
      found_slash = true;
    }
  }

  if (found_slash) {
    return(file_path.substr(final_slash_index+1, file_path.size()-final_slash_index-1));
  }
  else {
    return(file_path);
  }

}

// Function to remove the extension of the filename.
string remove_ext(string book_filename) {
  bool found_dot = false;
  int dot_index = book_filename.size();

  for (int i = 0; i < book_filename.size(); i++) {
    if (book_filename[i] == '.') {
      dot_index = i;
      found_dot = true;
    }
  }

  if (found_dot) {
    return(book_filename.substr(0, dot_index));
  }
  else {
    return(book_filename);
  }
}

// Function to check if a string starts with a prefix.
bool starts_with(string full_string, string prefix) {
  if (!full_string.empty() && !prefix.empty()) {
    string prefix_of_full_string = full_string.substr(0, prefix.size());
    if (prefix_of_full_string.compare(prefix) == 0) {
      return(true);
    }
    else {
      return(false);
    }
  }
  else {
    return(false);
  }
}

// Function to check if a string ends with a suffix.
bool ends_with(string full_string, string suffix) {
  if (!full_string.empty() && !suffix.empty()) {
    string suffix_of_full_string = full_string.substr(full_string.size()-suffix.size(), suffix.size());
    if (suffix_of_full_string.compare(suffix) == 0) {
      return(true);
    }
    else {
      return(false);
    }
  }
  else {
    return(false);
  }
}

// -- Main method

int main(int argc, char const *argv[]) {
    // Will be called as, formatText "ebook_with_extension"

    // If given a filename
    if (argc > 1) {
      // -- Get .txt files
      string book_filename = argv[1];

      // If .epub file, unzip and fetch .txt files
      if (ends_with(book_filename, ".epub")) {
        unzip_book(book_filename);
      }
      else {
        cerr << "File given is not of type: .epub" << endl;
      }

      // If other types here: TODO

      // -- Format .txt files into vector of useful chapters (chapterise)
      string book_name = remove_ext(get_filename_only(book_filename));
      vector<Chapter> chapters = chapterise(book_name);
    }
    else {
      cerr << "No file given, usage is: ./formatText \"ebook.epub\" " << endl;
    }

    return(0);
}
