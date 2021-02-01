import sys

import subprocess
import speech_recognition as sr

from tqdm import tqdm

import helpers

# script to take a whole wav file and split it into chunks of audio, then transcribe each one

def main(args):
    if len(args) != 1:
        sys.stderr.write(
            'Usage: wav_to_filelist.py <path to wav file>\n')
        sys.exit(1)

    path_to_wav = args[0]

    # split audio into chunks, using split_audio.py
    print("splitting wav into chunks..")
    split_audio_cmd = ['python', 'split_audio.py', '3', path_to_wav]
    subprocess.Popen(split_audio_cmd).wait()

    parent_path = helpers.get_parent_folder_from_path(path_to_wav)

    # get all audio chunks made
    audio_chunks_directory_path = parent_path + '/audio_chunks'
    audio_filepaths = helpers.get_audio_chunk_filepaths(audio_chunks_directory_path)

    # transcribe each of them and write to a text chunk file
    text_chunks_directory_path = parent_path + '/text_chunks'
    helpers.make_dir_if_nonexistent(text_chunks_directory_path)

    print("transcribing audio chunks..")
    chunks_that_need_manual = []
    for filepath in tqdm(audio_filepaths):
        filename = filepath.split('/')[-1]
        filename_with_txt = filename.split('.')[0] + '.txt'
        try:
            transcription = helpers.transcribe_wav_file(filepath)
            helpers.write_text_file(text_chunks_directory_path + '/' + filename_with_txt, transcription)
        # If failed to transcribe, add to list of chunks that need a manual text file made
        except sr.UnknownValueError:
            chunks_that_need_manual.append(filename_with_txt)
            # write a blank text file, for now
            helpers.write_text_file(text_chunks_directory_path + '/' + filename_with_txt, " ")

    if len(chunks_that_need_manual) != 0:
        # If any chunks couldn't be transcribed, don't build the filelist and instead report that they need manual entry
        print("Errors with the following chunks, please do these manually and then run 'chunks_to_filelist.py'")
        for chunk in chunks_that_need_manual:
            print(chunk)
    else:
        # run chunks_to_filelist
        chunks_to_filelist_cmd = ['python', 'chunks_to_filelist.py', parent_path]
        subprocess.Popen(chunks_to_filelist_cmd).wait()



if __name__ == '__main__':
    main(sys.argv[1:])
