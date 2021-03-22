import sys

import speech_recognition as sr
from pydub import AudioSegment
import helpers


# this approach doesn't give any punctuation at all, just text - not bad though
def main(args):
    if len(args) < 1 or len(args) > 2:
        sys.stderr.write(
            'Usage: transcribe_text_1.py <path to .wav file > <(optional) path to save text to>\n')
        sys.exit(1)

    # limit, if it exists
    outputting_to_file = False
    path_to_txt = ""
    if len(args) == 2:
        outputting_to_file = True
        path_to_txt = args[1]

    path_to_wav = args[0]

    transcription = ""

    recogniser = sr.Recognizer()
    with sr.AudioFile(path_to_wav) as source:
        audio = recogniser.record(source)
        transcription = recogniser.recognize_google(audio)

    if outputting_to_file:
        helpers.write_text_file(path_to_txt, transcription)
    else:
        print(transcription)

if __name__ == '__main__':
    main(sys.argv[1:])
