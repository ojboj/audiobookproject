#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <zip.h> // Need to install libzip

using namespace std;

// -- Helper functions.

// Delcarations
void unzip(string);
string remove_ext(string);
string sub_string(string, int, int);
bool ends_with(string, string);

// Function to unzip an epub file.
void unzip(string book_filename) {
    string book_path = "./" + book_filename;
    string extracted_book_path = "./" + remove_ext(book_filename) + "/";
    int err;
    struct zip *hZip = zip_open_w(book_path, 0, &err) {
        if (hZip) {
            size_t total_index = zip_get_num_entries(hZip, 0);
            for (size_t i = 0; i < total_index; i++) {
                struct zip_stat st;
                zip_stat_init(&st);
                zip_stat_index(hZip, i, 0, &st);

                struct zip_file *zf = zip_fopen_index(hZip, i, 0);
                if (!zf) {
                    zip_close(hZip);
                    return(false);
                }

                vector<char> buffer;
                buffer.resize(st.size);
                zip_fread(zf, buffer.data(), st.size);
                zip_fclose(zf);

                ofstream out_stream;
                string out_filename = extracted_book_path + to_string(i);
                out_stream.open(out_filename);
                for (int i = 0; i < buffer.size(); i++) {
                    out_stream << buffer[i];
                }
            }
        }
    }
}

// Function to remove the extension of the filename.
string remove_ext(string book_filename) {
    string filename_without_extension = "";

    int dot_index = book_filename.size();

    for (int i = 0; i < book_filename.size(); i++) {
        if (book_filename[i] == '.') {
            dot_index = i;
        }
    }

    return(sub_string(book_filename, 0, dot_index));
}

// Function to generate sub strings of strings.
string sub_string(string full_string, int start, int end) {
    string sub_string = "";

    if (start >= 0 && end < full_string.size()) {
        for (int i = start; i <= end; i++) {
            sub_string = sub_string + full_string[i];
        }
    }
    else {
        cout << " ERROR - sub_string: start and/or end out of bounds" << endl;
    }

    return(sub_string);
}

// Function to check if a string ends with a suffix.
bool ends_with(string full_string, string suffix) {
    if (!full_string.empty() && !suffix.empty()) {
        if (full_string.compare(full_string.rfind(suffix[0]), suffix.size(), suffix) == 0) {
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
    string book_filename = argv[1];

    unzip(book_filename);

    // vector<string> book_lines;
    // // Open file and store in vector
    // if (ends_with(book_filename, ".epub")) {
    //     ifstream in_file;
    //     string temp_line;
    //     in_file.open("./" + book_filename);
    //     if (in_file.is_open()) {
    //         cout << " Successfully opened file: " << book_filename << endl;
    //         while(getline(in_file, temp_line)) {
    //             book_lines.push_back(temp_line);
    //         }
    //     }
    //     else {
    //         cout << " ERROR - file: " << book_filename << " failed to open." << endl;
    //     }
    // }
    //
    // for (int i = 0; i < book_lines.size(); i++) {
    //     cout << book_lines[i] << endl;
    // }

    return(0);
}
