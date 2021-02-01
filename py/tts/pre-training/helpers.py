import contextlib
import os

from glob import glob

import speech_recognition as sr
from pydub import AudioSegment

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

def write_text_file(path, text):
    """Writes text to a .txt file.
    Takes the path, and writes the data to the file path.
    """
    with contextlib.closing(open(path, 'w')) as writer:
        return writer.write(text)

def transcribe_wav_file(path_to_wav):
    """Transcribes the spoken audio into a text file.
    Takes the path, and returns a string of the transcription.
    Will raise UnknownValueError() if failed (need manual).
    """
    recogniser = sr.Recognizer()
    with sr.AudioFile(path_to_wav) as source:
        audio = recogniser.record(source)
        transcription = recogniser.recognize_google(audio)
        return transcription

def get_parent_folder_from_path(path):
    """Gets the parent path from a given path
    """
    return '/'.join(path.split('/')[0:-1])

def make_dir_if_nonexistent(path_to_dir):
    """Creates a dir if the dir doesn't exist yet
    """
    if not os.path.exists(path_to_dir):
        os.mkdir(path_to_dir)

# function to get the audio chunks' filepaths
def get_audio_chunk_filepaths(parent_path, limit=0):
    """Returns filepaths that meet a .wav condition, can be limited optionally
    """
    # get the paths of the audio files
    filepaths = sorted(glob('{}/*.wav'.format(parent_path)))
    return filepaths[:limit if limit != 0 else len(filepaths)]
