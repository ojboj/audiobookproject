#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <zip.h>
#include <filesystem>

using namespace std;

// -- Helper functions.

// Delcarations.
void unzip_book(string);
string get_filename_only(string);
string remove_ext(string);
bool ends_with(string, string);

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

    // Get filename
    if (argc > 1) {
      string book_filename = argv[1];

      if (ends_with(book_filename, ".epub")) {
        unzip_book(book_filename);
      }
      else {
        cerr << "File given is not of type: .epub" << endl;
      }
    }
    else {
      cerr << "No file given, usage is: ./formatText \"ebook.epub\" " << endl;
    }

    return(0);
}
