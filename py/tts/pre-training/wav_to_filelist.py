import sys
import argparse
import os

import subprocess
import speech_recognition as sr

from tqdm import tqdm

import helpers

# script to take a whole wav file and split it into chunks of audio, then transcribe each one
def main(path_to_wav, args):
    # split audio into chunks, using split_audio.py
    print("splitting wav into chunks..")
    split_audio_cmd = ['python', 'split_audio.py', str(args.aggressiveness), path_to_wav]
    subprocess.Popen(split_audio_cmd).wait()

    parent_path = helpers.get_parent_folder_from_path(path_to_wav)

    # get all audio chunks made
    audio_chunks_directory_path = os.path.join(parent_path, 'audio_chunks')
    audio_filepaths = helpers.get_audio_chunk_filepaths(audio_chunks_directory_path)

    # transcribe each of them and write to a text chunk file
    text_chunks_directory_path = os.path.join(parent_path, 'text_chunks')
    helpers.make_dir_if_nonexistent(text_chunks_directory_path)

    print("transcribing audio chunks..")
    chunks_that_need_manual = []
    for filepath in tqdm(audio_filepaths):
        _, filename_with_wav = os.path.split(filepath)
        filename_with_txt = filename_with_wav.split('.')[0] + '.txt'
        try:
            transcription = helpers.transcribe_wav_file(filepath)
            helpers.write_text_file(os.path.join(text_chunks_directory_path, filename_with_txt), transcription)
        # If failed to transcribe, add to list of chunks that need a manual text file made
        except sr.UnknownValueError:
            # If not flagging to skip untranscribables, add to running list and store an empty file
            if not args.skip_untrans:
                chunks_that_need_manual.append(filename_with_txt)
                # write a blank text file, for now
                helpers.write_text_file(os.path.join(text_chunks_directory_path, filename_with_txt), " ")
            # Else do nothing, aka skip

    if len(chunks_that_need_manual) != 0:
        # If any chunks couldn't be transcribed, don't build the filelist and instead report that they need manual entry
        print("Errors with the following chunks, please do these manually and then run 'chunks_to_filelist.py'")
        for chunk in chunks_that_need_manual:
            print(chunk)
    else:
        # run chunks_to_filelist
        chunks_to_filelist_cmd = ['python', 'chunks_to_filelists.py', parent_path]
        subprocess.Popen(chunks_to_filelist_cmd).wait()



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='turn a single audio file of spoken word into a filelist suitable for tacotron2')
    parser.add_argument('-i', '--input_wav', type=str, required=True,
                        help='audio file, in .wav format, to form filelist for')
    parser.add_argument('-s', '--skip_untrans', action='store_true', default=False,
                        help='flag to indicate that you wish to skip audio chunks that couldn\'t be transcribed')
    parser.add_argument('-a', '--aggressiveness', type=int, required=False, default=3,
                        help='int to indicate how aggresively you wish to split the audio file at pauses, 0-3')

    args = parser.parse_args()
    path_to_wav = args.input_wav

    main(path_to_wav, args)
