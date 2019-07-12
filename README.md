# audiobookproject
Take in an ebook and output an audiobook. Different, matching, voices for dialogue of main speakers.

# Current working notes:

## Compile with:
g++ -I /usr/local/include/ -L /usr/local/lib/ formatText.cpp -o formatText -lzip -std=c++17

## Run with:
./formatText "books/A Black Comedy - nonjon.epub"

## What's next:
replace generated .txt files with new .txt files from generated Chapter objects, super clean and clear, ready for TTS. - done.

Use clean .txt files to generate TTS audio files (m4b?) for each chapter.

Add the ability to keep bold and emphasised text in the document for better TTS.

Generate a list of characters.

Locate and link dialogue to the character that speaks it.

Locate and link any description(s) of a character to the character.

Analyse gathered info on a character to pick out a voice for them (ML).

### Low Priority

Don't overwrite html_files if they already exist.
