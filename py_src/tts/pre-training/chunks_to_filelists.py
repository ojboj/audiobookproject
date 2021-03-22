import sys
import os
import math
import random

from glob import glob

import helpers

# function to build the filelist from audio and text chunks
def pairs_to_text(pairs, train_ratio):
    textfiles = {}

    # splitting pairs into train and val
    length_of_pairs = len(pairs)
    split_index = int(math.floor(train_ratio*length_of_pairs))
    shuffled_pairs = random.sample(pairs, k=length_of_pairs)

    train_pairs = shuffled_pairs[:split_index]
    val_pairs = shuffled_pairs[split_index:]

    # for train pairs
    train_textfile = ""
    for pair in train_pairs:
        train_textfile += ("{}|{}\n".format(pair["file_path"], pair["chunk_text"]))

    # for val pairs
    val_textfile = ""
    for pair in val_pairs:
        val_textfile += ("{}|{}\n".format(pair["file_path"], pair["chunk_text"]))

    # add to object
    textfiles["train"] = train_textfile
    textfiles["val"] = val_textfile
    return textfiles

def get_text_chunk_string(filepath):
    file_as_string = helpers.read_text_file(filepath)
    return helpers.clean_text(file_as_string)

# function to form pairs from the audio and text chunks
def form_pairs_from_texts_and_filepaths(text_filepaths, audio_filepaths):
    pairs = []

    # For each text filepath, get its corresponding audio chunk path and pair it
    for text_filepath in text_filepaths:
        # get the chunk number
        chunk_number = helpers.get_chunk_number_from_filepath(text_filepath)
        # get any audio chunks that match the chunk number
        matching_audio_chunks = [path for path in audio_filepaths if helpers.get_chunk_number_from_filepath(path) == chunk_number]
        # if there is more than one, throw
        if len(matching_audio_chunks) > 1:
            sys.stderr.write(
                'Too many audio chunks found for chunk number: {}'.format(chunk_number))
            sys.exit(1)
        # else if there is one, add to pairs
        if len(matching_audio_chunks) == 1:
            pair = {}
            pair["chunk_text"] = get_text_chunk_string(text_filepath)
            # tacotron2 needs '/' not OS_SEPARATOR in the filelists, so need to do a replace
            pair["file_path"] = matching_audio_chunks[0].replace(helpers.OS_SEPARATOR, '/')
            pairs.append(pair)
        # else, move on
    return pairs

def main(args):
    if len(args) < 1 or len(args) > 2:
        sys.stderr.write(
            'Usage: chunks_to_filelist.py <path to parent chunk folders> <(optional) number of chunks to limit to>\n')
        sys.exit(1)

    # limit, if it exists
    limit = 0
    if len(args) == 2:
        limit = int(args[1])

    # parent path
    parent_path = args[0]

    # get the audio file audio_filepaths
    audio_filepaths = helpers.get_audio_chunk_filepaths(os.path.join(parent_path, "audio_chunks"), limit)

    # get the text chunk filepaths
    text_chunk_filepaths = helpers.get_text_chunk_filepaths(os.path.join(parent_path, "text_chunks"), limit)

    # form into pairs
    pairs = form_pairs_from_texts_and_filepaths(text_chunk_filepaths, audio_filepaths)

    # turn pairs into tacotron2 filelists/one string
    train_ratio = 0.75 # TODO: make this an arg
    textfiles = pairs_to_text(pairs, train_ratio)

    # write to train and val filelists files
    helpers.write_text_file(os.path.join(parent_path, "train_filelist.txt"), textfiles["train"])
    helpers.write_text_file(os.path.join(parent_path, "val_filelist.txt"), textfiles["val"])


if __name__ == '__main__':
    main(sys.argv[1:])
