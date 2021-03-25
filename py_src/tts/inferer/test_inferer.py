import sys
import os

from inferer import Inferer

def main(args):
    if len(args) != 3:
        sys.stderr.write(
            'Usage: test_inferer.py <path to tacotron2 pt> <path to waveglow pt> <text phrase>\n')
        sys.exit(1)

    tacotron2_path = args[0]
    waveglow_path = args[1]
    phrase = args[2]

    sample_rate = 16000 # could make arg

    inferer = Inferer(tacotron2_path, waveglow_path, sample_rate)

    audio_1_numpy = inferer.infer('oh,')
    audio_2_numpy = inferer.infer('hi mark.')

    stitched_audio_numpy = inferer.stitch_audio(audio_1_numpy, audio_2_numpy)
    inferer.write_wav('stitched.wav', stitched_audio_numpy)


if __name__ == '__main__':
    main(sys.argv[1:])
