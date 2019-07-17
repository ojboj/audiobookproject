#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <zip.h>
#include "Chapter.h"
#include "StringHelper.h"

using namespace std;

// Class to get .txt files from .epub file.
class FormatText {
  public:
    // Function to get chapters to a vector and also format the .txt files.
    static vector<Chapter> get_chapters(string book_filename) {
      vector<Chapter> chapters;
      bool unzipped = false;

      // If .epub file, unzip and fetch .txt files
      if (StringHelper::ends_with(book_filename, ".epub")) {
        unzipped = unzip_book(book_filename);
      }
      // If other types, here: TODO
      else {
        cerr << "File given is not of type: .epub" << endl;
      }

      // If successfully unzipped
      if (unzipped) {
        // -- Format .txt files into vector of useful chapters (chapterise)
        string book_name = remove_ext(get_filename_only(book_filename));
        chapters = chapterise(book_name);

        // Ouput generated chapter objects to .txt files
        cleanup_txt_files(book_name, chapters);
      }

      // Return chapters
      return(chapters);
    }

  // -- Helper functions.
  private:
    // Function to change all html_fileX.txt to chapterX.txt.
    static void cleanup_txt_files(string book_name, vector<Chapter> chapters) {
      // Get folder path
      string chapters_folder_path = "./" + book_name + "/text/";
      filesystem::path path = chapters_folder_path;

      // Remove folder and all html_fileX.txt from directory
      filesystem::remove_all(path);
      // Create an empty directory
      filesystem::create_directory(path);

      // If directory is successfully made
      if (filesystem::exists(path)) {
        // Iterate through each Chapter object
        for (int i = 0; i < chapters.size(); i ++) {
          // Generating path
          string out_file_path = chapters_folder_path + "chapter" + to_string(i) + ".txt";
          // Opening file to write to
          ofstream out_file;
          out_file.open(out_file_path);
          // Writing to file
          // out_file << "chapter_title:" << endl;
          out_file << chapters[i].title << endl;
          //
          // out_file << "chapter_disclaimer:" << endl;
          // out_file << chapters[i].disclaimer << endl << endl;;

          // out_file << "chapter_body:" << endl;
          out_file << chapters[i].body << endl;

          out_file.close();
        }
      }
    }

    // Function to turn folder of .txt files into chapter objects.
    static vector<Chapter> chapterise(string book_name) {
      vector<Chapter> chapters;
      int count_of_chapters = 0;
      string text_folder_path = book_name + "/text";
      filesystem::path path = text_folder_path;

      if (filesystem::exists(path)) {
        // Iterate through all files in the folder
        for(auto& p: filesystem::directory_iterator(text_folder_path)) {
          // If .txt file
          if (StringHelper::ends_with(p.path().string(), ".txt")) {
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
            string raw_body = "";

            // Check file_buffer data to see if chapter
            for (int i = 0; i < file_buffer.size(); i++) {

              // Line signifying a chapter is unique at this point
              if (StringHelper::starts_with(file_buffer[i], "<h2 style")) {
                // We have a chapter bois
                is_chapter = true;

                // Get chapter number from file name
                string file_name = remove_ext(get_filename_only(p.path().string()));
                string chapter_number_string = "";
                // Check if correct file
                if (StringHelper::contains(file_name, "html_file")) {
                  // Get string for html file number
                  for (int j = 9; j < file_name.size(); j++) {
                    chapter_number_string = chapter_number_string + file_name[j];
                  }
                }
                else {
                  cerr << "chapterise: .txt file is not called html_fileX, where X is a number" << endl;
                }
                if (!chapter_number_string.empty()) {
                  chapter_number = stoi(chapter_number_string);
                }
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
              // If is_chapter, rest of file is body
              else if (is_chapter) {
                raw_body = raw_body + file_buffer[i] + " ";
              }
            }
            // Remove formatting from body
            string body = remove_formatting(raw_body);

            // Info gathered for chapter, make the Chapter object
            if (is_chapter) {
              chapters.push_back(Chapter(chapter_number, chapter_title, body));
            }
          }
        }
      }
      else {
        cerr << "chapterise: text folder doesn't exist" << endl;
      }

      // Return sorted vector of chapters
      sort(chapters.begin(), chapters.end());
      return(chapters);
    }

    // Function to remove any formatting from text.
    static string remove_formatting(string raw_html) {
      string text = "";
      for (int i = 0; i < raw_html.size(); i++) {
        // If you find any html formatting
        if (raw_html[i] == '<') {
          bool new_line = false;
          // If find <p>, mark for replacement with \n
          if (raw_html[i+1] == 'p' && raw_html[i+2] == '>') {
            new_line = true;
          }
          // Find where it ends and begin loop from there, skipping it
          int j = i+1;
          while (raw_html[j] != '>' && j < raw_html.size()) {
            j++;
          }
          // If marked for new line, add \n
          if (new_line) {
            text = text + '\n';
          }
          // // Else, add a space
          // else {
          //   text = text + ' ';
          // }
          i = j;
        }
        else if (raw_html[i] == '\n') {
          text = text + ' ';
        }
        // If not a double space, add to text
        else if (!(raw_html[i-1] == ' ' && raw_html[i] == ' ')){
          text = text + raw_html[i];
        }
      }

      // Remove unnecessary \n chars
      while (text[text.size()-1] == '\n') {
        text = text.substr(0, text.size()-1);
      }
      return(text);
    }

    // Function to unzip an epub file.
    static bool unzip_book(string book_filename) {
      bool unzipped = false;

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
      // If successful
      if (filesystem::exists(path)) {
        filesystem::path text_path = chapters_folder_path + "text/";
        filesystem::create_directory(text_path);
        // If successful
        if (filesystem::exists(text_path)) {
          // Open zip file
          int err = 0;
          zip *z = zip_open(book_path, 0, &err);
          // Succeeded to open the file
          if (!(z == NULL)) {
            // loop through chapters here
            int number_of_files = zip_get_num_entries(z, 0);
            int count_of_html_files = 0;

            for (int i = 0; i < number_of_files; i++) {
              // Get i-th file
              const char* name = zip_get_name(z, i, 0);

              // Test if .html
              string name_of_file = name;
              if (StringHelper::ends_with(name_of_file, ".html")) {
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
                string out_file_path = chapters_folder_path + "text/html_file" + to_string(count_of_html_files) + ".txt";
                if (!ofstream(out_file_path).write(contents, st.size)) {
                  cerr << "unzip_book: could not write file: " << name_of_file << endl;
                }
                count_of_html_files ++;
              }
            }

            zip_close(z);
            unzipped = true;
          }
        }
      }

      return(unzipped);
    }

    // Function to get the filename only.
    static string get_filename_only(string file_path) {
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
    static string remove_ext(string book_filename) {
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
};
