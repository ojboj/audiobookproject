#include <iostream>
#include "Chapter.h"
#include "FormatText.h"
// #include "TextToSpeech.h"
#include "BookAnalysis.h"

using namespace std;

// -- Main method

int main(int argc, char const *argv[]) {
    // Will be called as, formatText "ebook_with_extension"

    // If given a filename
    if (argc > 1) {
      // -- Create and format .txt files, use to generate vector of Chapter objects
      string book_filename = argv[1];
      vector<Chapter> chapters = FormatText::get_chapters(book_filename);

      // Use chapters to generate the audio book files
        // Should be trivial to just use a TTS application to read out .txt files

      // Generate a list of characters in the entire book with an indication of their relevance
        // Either through simply the number of times the name comes up or through the number of dialogue associated with them
          // Number of dialogue is more useful for picking which characters to voice
          // Generate a sorted vector of characters and an array of their dialogue, order by quantity of associated dialogue
      BookAnalysis::get_characters(chapters);
    }
    else {
      cerr << "No file given, usage is: ./audioproj \"ebook.epub\" " << endl;
    }

    return(0);
}
