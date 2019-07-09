# audiobookproject
Take in an ebook and output an audiobook. Different, matching, voices for dialogue of main speakers.

# Current working notes:

## Compile with:
g++ -I /usr/local/include/ -L /usr/local/lib/ formatText.cpp -o formatText -lzip -std=c++17

## Run with:
./formatText "books/A Black Comedy - nonjon.epub"

## What's next:
replace generated .txt files with new .txt files from generated Chapter objects, super clean and clear, ready for TTS
