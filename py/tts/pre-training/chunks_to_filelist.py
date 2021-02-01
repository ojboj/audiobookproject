import sys

from glob import glob

import helpers

# function to build the filelist from audio and text chunks
def pairs_to_text(pairs):
    textfile = ""
    for pair in pairs:
        textfile += ("{}|{}\n".format(pair["file_path"], pair["chunk_text"]))

    return textfile

# function to form pairs from the audio and text chunks
def form_pairs_from_texts_and_filepaths(text_chunks, audio_filepaths):
    if len(text_chunks) != len(audio_filepaths):
        sys.stderr.write('Incompatible lengths for text and audio chunks')
        sys.exit(1)
    pairs = []
    for index, text_chunk in enumerate(text_chunks):
        pair = {}
        pair["chunk_text"] = text_chunk
        pair["file_path"] = audio_filepaths[index]
        pairs.append(pair)
    return pairs

# function to get the text/quote for each text chunk
def get_text_chunk_strings(parent_path, limit=0):
    # open each text chunk and put its trimmed text into an array
    filepaths = sorted(glob('{}/*.txt'.format(parent_path)))

    text_chunk_strings = []

    for path in filepaths[:limit if limit != 0 else len(filepaths)]:
        file_as_string = helpers.read_text_file(path)
        text_chunk_strings.append(helpers.clean_text(file_as_string))

    return text_chunk_strings

def main(args):
    if len(args) < 1 or len(args) > 2:
        sys.stderr.write(
            'Usage: chunks_to_filelist.py <path to parent chunk folders> <(optional) number of chunks>\n')
        sys.exit(1)

    # limit, if it exists
    limit = 0
    if len(args) == 2:
        limit = int(args[1])

    # parent path
    parent_path = args[0]

    # get the audio file audio_filepaths
    audio_filepaths = helpers.get_audio_chunk_filepaths(parent_path + "/audio_chunks", limit)

    # get the text chunks
    text_chunk_strings = get_text_chunk_strings(parent_path + "/text_chunks", limit)

    # form into pairs
    pairs = form_pairs_from_texts_and_filepaths(text_chunk_strings, audio_filepaths)

    # turn pairs into tacotron2 filelists/one string
    textfile = pairs_to_text(pairs)

    # write to filelists file
    helpers.write_text_file(parent_path + "/filelist.txt", textfile)


if __name__ == '__main__':
    main(sys.argv[1:])
