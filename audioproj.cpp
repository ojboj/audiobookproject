#include <iostream>
#include "Chapter.h"
#include "FormatText.h"
// #include "TextToSpeech.h"

using namespace std;

// -- Main method

int main(int argc, char const *argv[]) {
    // Will be called as, formatText "ebook_with_extension"

    // If given a filename
    if (argc > 1) {
      // -- Get .txt files
      string book_filename = argv[1];
      vector<Chapter> chapters = FormatText::get_chapters(book_filename);

      // Use chapters to generate the audio book files
      
    }
    else {
      cerr << "No file given, usage is: ./formatText \"ebook.epub\" " << endl;
    }

    return(0);
}
