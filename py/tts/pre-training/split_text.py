import contextlib
import sys
import os

import nltk

def read_text_file(path):
    """Reads a .txt file.
    Takes the path, and returns the whole file.
    """
    with contextlib.closing(open(path, 'r')) as reader:
        return reader.read()


def clean_text(text):
    """Trims whitespace and newlines.
    """
    return ' '.join(text.split())


def get_sentences(text):
    """Gets sentences from text input.
    Takes the text, and returns a list of sentences, using nltk.
    """
    return nltk.sent_tokenize(clean_text(text))


def write_text_file(path, text):
    """Writes text to a .txt file.
    Takes the path, and writes the data to the file path.
    """
    with contextlib.closing(open(path, 'w')) as writer:
        return writer.write(text)


def main(args):
    if len(args) != 1:
        sys.stderr.write(
            'Usage: split_text.py <path to wav file>\n')
        sys.exit(1)
    # Read the text file
    text = read_text_file(args[0])

    # Get sentences
    sentences = get_sentences(text)

    # Creating path to chunks directory
    dir_path = '/'.join(args[0].split('/')[0:-1]) + '/text_chunks'
    # Creating chunks directory, if it doesn't exist yet
    if not os.path.exists(dir_path):
        os.mkdir(dir_path)
    # Writing each audio file
    for i, sentence in enumerate(sentences):
        file_name = '/chunk-%002d.txt' % (i,)
        write_text_file(dir_path + file_name, sentence)


if __name__ == '__main__':
    main(sys.argv[1:])
