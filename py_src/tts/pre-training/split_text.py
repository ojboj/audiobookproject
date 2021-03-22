import contextlib
import sys
import os

import nltk

import helpers

def get_sentences(text):
    """Gets sentences from text input.
    Takes the text, and returns a list of sentences, using nltk.
    """
    return nltk.sent_tokenize(helpers.clean_text(text))

def main(args):
    if len(args) != 1:
        sys.stderr.write(
            'Usage: split_text.py <path to wav file>\n')
        sys.exit(1)

    text_file_path = args[0]

    # Read the text file
    text = helpers.read_text_file(text_file_path)

    # Get sentences
    sentences = get_sentences(text)

    # Creating path to chunks directory
    dir_path = os.path.join(os.path.dirname(text_file_path), 'text_chunks')
    # Creating chunks directory, if it doesn't exist yet
    if not os.path.exists(dir_path):
        os.mkdir(dir_path)
    # Writing each audio file
    for i, sentence in enumerate(sentences):
        file_name = '/chunk-%00{}d.wav'.format(len(str(len(sentences)-1))) % (i,)
        helpers.write_text_file(dir_path + file_name, sentence)


if __name__ == '__main__':
    main(sys.argv[1:])
